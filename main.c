#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int int_constant_mi(char *);
int keywords(char *);
int gecerli_mi(char );
int print_mi(char dizi[]);
void isaretler(char dizi[]);
int lexeme_kontrol(char dizi[],int k);
void dosya_oku(FILE *);
FILE *ptr_2;
FILE *ptr_1;
int main()
{
    FILE *ptr;
    char dosya_adi[20];
    char karakter;
    int i=0,key=0,sayac=0,k=0,satir_say=1;
    printf("dosya adini giriniz:");//kullanicidan dosya adini istiyoruz
    scanf("%s",dosya_adi);
    while(dosya_adi[k]!='\0')
    k++;
    dosya_adi[k]='.';
    dosya_adi[k+1]='f';
    dosya_adi[k+2]='o';
    dosya_adi[k+3]='r';
    dosya_adi[k+4]='\0';
    ptr=fopen(dosya_adi,"r");
    if(ptr==NULL)//dosya açýlamadý ise...
        printf("dosya acilamadi!");
    else//dosya açildiysa...
    {
        ptr_2=fopen("a.txt","w");
        karakter=getc(ptr);
        while(karakter!=EOF)//dosya sonuna kadar okuyabilmem için gerekli
        {
            char lexeme[100]= {'\0'};
            if(isdigit(karakter))//okunan karakterin sayi olma olasýlýðý
            {
                i=0;
                lexeme[i++]=karakter;//eger karakter bir rakamsa lexeme dizimize ekliyoruz.
                karakter=getc(ptr);//ve bir sonraki karakteri aliyoruz.
                while((isdigit(karakter) && karakter!=10)||karakter=='.')//karakter rakam oldugu sürece ve null olmadiği sürece diziye  eleman ekle
                {
                    lexeme[i++]=karakter;
                    karakter=getc(ptr);
                }
                lexeme[i]='\0';//ve lexeme dizisini sonuna null ata,lexeme dizisini yazdirirken önemli..
                if(karakter=='\0')//eger satir sonu isaretini gördüysen
                    satir_say++;//satir sayisini arttir.Böylece nerde hata olduğunu bilebiliriz.
                if(lexeme_kontrol(lexeme,i+1)==1)//lexeme dizisinin int const mu real const mu olduğunu bulan fonksiyon
                {
                    fprintf(ptr_2,"real_constant (%s)\n",lexeme);
                    printf("real_constant (%s)\n",lexeme);
                }
                else
                {
                    fprintf(ptr_2,"int constant (%s)\n",lexeme);
                    printf("int constant (%s)\n",lexeme);
                }
                continue;
            }
            if(isalpha(karakter))//eger ki karakter harf ise
            {
                i=0;
                if(karakter==67)//yani yorum satırı ise
                {
                    lexeme[i++]=karakter;
                    karakter=getc(ptr);
                    if(isspace(karakter))
                    {
                        while(karakter!=10)//satir sonunu görene kadar devam et.

                        {
                            karakter=getc(ptr);
                            if(karakter==10)//eger satir sonunu görürsen
                                satir_say++;//satir sayisini arttir
                        }
                        continue;
                    }
                }

                lexeme[i++]=karakter;
                sayac=1;//en başta bir karakter aldigimiz için sayac 1 den baslar
                karakter=getc(ptr);
                while((isalpha(karakter)&&karakter!=10)||karakter=='_' ||isdigit(karakter))
                {
                    sayac++;//kelimenin uzunluğunu tutmamız icin gerekli olan sayac
                    lexeme[i++]=karakter;
                    karakter=getc(ptr);
                }
                lexeme[i]='\0';
                key=keywords(lexeme);

                if(sayac>32)// fortranda bir kelime max 32 karakterden oluşur
                {
                    fprintf(ptr_2," %d . satirda HATA!! cok uzun bu kelime...\n",satir_say);
                    printf(" %d . satirda HATA!! cok uzun bu kelime...\n",satir_say);
                    continue;
                }
                if(karakter==10)
                    satir_say++;

                if(print_mi(lexeme)==1)// fortranda print * deyimini kontrol eden fonksiyon çagrilir.
                {
                    karakter=getc(ptr);
                    if(karakter==32)
                        lexeme[i++]=karakter;
                    while(isspace(karakter))//print kelimesinden sonra gelebilecek bosluklari atlamaliyiz.
                    {
                        karakter=getc(ptr);
                    }
                    if(karakter=='*')
                        lexeme[i++]=karakter;
                    lexeme[i]='\0';
                    karakter=getc(ptr);
                    fprintf(ptr_2,"keyword (%s)\n",lexeme);
                    printf("keyword (%s)\n",lexeme);
                    continue;
                }
                else if(key==1)//eger kelime bir special word ise
                {
                    fprintf(ptr_2,"keyword (%s )\n",lexeme);
                    printf("keyword (%s )\n",lexeme);
                    continue;
                }
                else
                {
                    fprintf(ptr_2,"identifier (%s)\n",lexeme);
                    printf("identifier (%s)\n",lexeme);
                    continue;
                }
            }

            if(karakter==39)//karakterin ' ile baslamasi yani string olma olasiligi icin gerekli
            {
                i=0;
                lexeme[i++]=karakter;
                karakter=getc(ptr);
                while(karakter!=39 && karakter!=EOF && karakter!=10)
                {
                    lexeme[i++]=karakter;
                    karakter=getc(ptr);
                }
                if(karakter==10)
                   {satir_say++;
                    karakter=getc(ptr);

                   }

                if(karakter==39)//eger ki birdaha ' karakteri gelmisse yani string dogru bir sekilde tamamlanmis ise
                {
                    lexeme[i++]=karakter;
                    lexeme[i]='\0';
                    fprintf(ptr_2,"stringConstant %s\n",lexeme);
                    printf("stringConstant %s\n",lexeme);
                }
                if(karakter==-1)//yada ' karakteri hiç gelmedi yani 'ali gibi tamamlanmamis bir string ise....
                {
                    fprintf(ptr_2,"%d . satirda  HATA!!!tamamlanmamiş string sabiti...\n",satir_say-1);
                    printf("%d .satirda HATA!!!tamamlanmamiş string sabiti...\n",satir_say-1);
                }
                karakter=getc(ptr);
                continue;
            }
            if(gecerli_mi(karakter))//gecersiz bir karakterle baslıyorsa
            {
                fprintf(ptr_2,"gecersiz bir karakter!!\n");
                printf("gecersiz bir karakter!!\n");
                continue;
            }
            if(isspace(karakter)&& karakter!=10)//karakter bosluksa
            {
                karakter=getc(ptr);
                while(isspace(karakter)&& karakter!=10)
                {
                    karakter=getc(ptr);
                }
                if(karakter==10)
                    satir_say++;
                continue;
            }
            if(karakter=='_')//karakter _ ise
            {
               fprintf(ptr_2,"underscore_op(%c) \n",karakter);
                printf("underscore_op(%c) \n",karakter);
                karakter=getc(ptr);
                continue;
            }
            if(karakter==43)// karakter + sembolü ise
            {
                fprintf(ptr_2,"Increament_op (%c) \n",karakter);
                printf("Increament_op (%c) \n",karakter);
                karakter=getc(ptr);
                continue;
            }
            if(karakter=='-')//karakter - sembolü ise
            {
                fprintf(ptr_2,"eksi_op (%c)\n",karakter);
                printf("eksi_op (%c)\n",karakter);
                karakter=getc(ptr);
                continue;
            }
            if(karakter=='/')//karakter / sembolü ise
            {
                fprintf(ptr_2,"slash_op(%c)\n",karakter);
                printf("slas_op (%c)\n",karakter);
                karakter=getc(ptr);
                continue;
            }
            if(karakter=='(')//karakter ( sembolü ise
            {
                fprintf(ptr_2," open_paran \n");
                printf(" open_paran \n");
                karakter=getc(ptr);
                continue;
            }
            if(karakter==')')//karakter ) sembolü ise
            {
                fprintf(ptr_2,"closed_paran\n");//parantezin kapanmasını kontrol edecek miyiz?SORRRR
                printf("closed_paran\n");
                karakter=getc(ptr);
                continue;
            }
            if(karakter==',')// karakter , sembolü ise
            {
                fprintf(ptr_2,"comma\n");
                printf("comma\n");
                karakter=getc(ptr);
                continue;
            }
            if(karakter=='=')//karakter = sembolü ise
            {
                fprintf(ptr_2,"atama_op\n");
                printf("atama_op\n");
                karakter=getc(ptr);
                continue;
            }
            if(karakter=='*')//karakter * sembolü ise
            {
                karakter=getc(ptr);
                if(karakter=='*')
                {
                    fprintf(ptr_2,"us_alma_op\n");
                    printf("us_alma_op\n");
                    karakter=getc(ptr);
                }
                else
                {
                    fprintf(ptr_2,"astrix_op\n");
                    printf("astrix_op\n");
                }

                continue;
            }
            if(karakter==46)//karakter . ise
            {
                i=0;
                lexeme[i++]=karakter;
                karakter=getc(ptr);
                while(karakter!=46)
                {
                    lexeme[i++]=karakter;
                    karakter=getc(ptr);
                }
                lexeme[i++]=karakter;
                lexeme[i]='\0';
                isaretler(lexeme);//<,<=,>,>= sembolleri fortranda .le. gibi sembollerle ifade edilir .Bu gorevi yapan fonk cagriliyor
                karakter=getc(ptr);
                continue;
            }
            if(karakter==10)//eger satir sonuna gelmissek
            {
                satir_say++;
                karakter=getc(ptr);
                while(karakter==10)
                {
                    satir_say++;
                    karakter=getc(ptr);
                }
                continue;
            }
        }
        fclose(ptr_2);//yazma yaptigimiz dosya kapanir

    }
    return 0;
}
int lexeme_kontrol(char dizi[],int k)
{
    int i=0,bulundu=0;
    for(i=0; i<k; i++)
    {
        if(dizi[i]=='.')
        {
            bulundu=1;
            break;
        }
    }
    if(bulundu==1)
        return 1;
    else return 0;
}
int keywords(char dizi[])// keywordlarý tanýtmak için bir fonksiyon
{
    if(strcmp("PROGRAM",dizi)==0 || strcmp("INTEGER",dizi)==0 || strcmp("READ",dizi)==0 ||
            strcmp("CALL",dizi)==0 || strcmp("EXTERNAL",dizi)==0 || strcmp("COMMON",dizi)==0||strcmp("END",dizi)==0
            || strcmp("SUBROUTINE",dizi)==0 ||strcmp("DATA",dizi)==0 ||strcmp("IF",dizi)==0 ||strcmp("THEN",dizi)==0
            || strcmp("END IF",dizi)==0 ||strcmp("DO",dizi)==0 ||strcmp("END DO",dizi)==0 || strcmp("REAL",dizi)==0
            || strcmp("DATA",dizi)==0 || strcmp("CONTINUE",dizi)==0 || strcmp("STOP",dizi)==0 || strcmp("DOUBLE",dizi)==0
            || strcmp("FUNCTION",dizi)==0 || strcmp("ELSE",dizi)==0 || strcmp("RETURN",dizi)==0 )
        return 1;
    else return 0;
}
int print_mi(char dizi[])
{
    if(strcmp("PRINT",dizi)==0)
        return 1;
    else return 0;
}
int gecerli_mi(char hrf)//bir karakterin geçerli olup olmaýðýna bakar
{
    if(hrf==35 ||hrf==36 || hrf==37 ||hrf==38 ||hrf==94 ||hrf==62 ||hrf==60 ||hrf==33|| hrf==64)
        return 1;
    else
        return 0;
}
void isaretler(char dizi[])
{
    if(strcmp(".LT.",dizi)==0)
    {
        fprintf(ptr_2,"kucuktur_isareti\n");
        printf("kucuktur_isareti\n");
    }

    if(strcmp(".LE.",dizi)==0)
    {
        fprintf(ptr_2,"kucuk_esittir_isareti\n");
        printf("kucuk_esittir_isareti\n");
    }
    if(strcmp(".GT.",dizi)==0)
    {
        fprintf(ptr_2,"buyuktur_isareti\n");
        printf("buyuktur_isareti\n");
    }
    if(strcmp(".GE.",dizi)==0)
    {
        fprintf(ptr_2,"buyuk_esittir_isareti\n");
        printf("buyuk_esittir_isareti\n");
    }
    if(strcmp(".EQ.",dizi)==0)
    {
        fprintf(ptr_2,"esittir_isareti\n");
        printf("esittir_isareti\n");
    }
    if(strcmp(".NE.",dizi)==0)
    {
        fprintf(ptr_2,"esit_degildir_isareti\n");
        printf("esit_degildir_isareti\n");
    }

}












