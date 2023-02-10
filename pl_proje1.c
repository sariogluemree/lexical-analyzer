#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//metodlar
void resetToken(char text[], int tokenSize);
void lower(char text[]);
void upper(char text[]);
int controlKeyword(char* token, FILE *lex);
int controlInteger(char* token, int tokenSize,FILE *lex);
void writeIdentifier(char* token, int tokenSize,FILE *lex);

int main()
{
    // dosya işlemleri
    FILE *psi,*lex;
    if((psi = fopen("code.psi","r"))==NULL)
    {
        printf("%s","File not found.");
        return -1;
    }
    lex = fopen("code.lex","w");

    // sınırlayıcı karakterlerin tutulduğu 'delimiters' arrayi oluşturuldu.
    char delimiters[] = {'+','-','*','/',':','=','(',')','[',']','{','}',' ',';','"','\t',','};

    // dosyadan okunan satırın tutulacağı 'line' arrayi oluşturuldu.
    char line[250] = {0};

    // sınırlayıcı bir karaktere denk gelene kadar okunan karakterlerin tutulacağı 'token' arrayi oluşturuldu.
    char token[250] = {0};

    // token içerisinde kaç adet karakter olduğunu tutmak için 'tokenSize' değişkeni oluşturuldu.
    int tokenSize = 0;

    // Satır içerisindeki değerler sorgulanırken sınırlayıcı(delimiters) değerlerden geçip geçmediğimizi ve
    // içerisinde olup olmadığımızı anlamak için birçok flag değişkeni kullanıldı.
    bool stringFlag         = false;
    bool commentFlag        = false;
    bool bracketFlag        = false;
    bool operatorFlag       = false;
    bool doubleOperatorFlag = false;
    bool spaceFlag          = false;
    bool endOfLine          = false;

    // fgets ve while döngüsüyle her satırın okunması.
    while(fgets(line, sizeof line, psi))
    {
        // okunan satırı karakter karakter okumak için for döngüsü oluşturuldu.
        for(int i = 0; line[i] != '\0'; i++)
        {
            // art arda gelen iki operatörün kontrolü için ör.(++,--,:=) flag kontrolü.
            if(doubleOperatorFlag)
            {
                doubleOperatorFlag = false;
                continue;
            }

            // comment içerisinde olup-olmadığımızı anlamak için bir flag kontrolü.
            if(commentFlag)
            {
                if(line[i] == ')' && line[i-1] == '*')
                {
                    commentFlag = false;
                }
                continue;
            }

            // okunan değerin string olup-olmadığının kontrolü.
            if(delimiters[14] == line[i])
            {
                if(stringFlag)
                {
                    fprintf(lex,"%s(%s)\n","String",token); // "String" tokenın yazdırılması
                    resetToken(token, tokenSize);  // token char dizisinin boşaltılması
                    tokenSize=0;
                    stringFlag = false;
                    continue;
                }

                else if(!stringFlag)
                {
                    stringFlag = true;
                    continue;
                }
            }

            if(stringFlag)
            {
                token[tokenSize] = line[i];
                tokenSize++;
                continue;
            }

            // sınırlayıcı(delimiter) karakterlerin ne olduğunu kontrol etmek için bir for döngüsü daha kullanıldı
            // delimiters arrayinin uzunluğu kadar dönen for döngüsü
            for(int j = 0; j<17; j++)
            {
                // sınırlayıcılar arasından opearatörlerin seçilmesi için if koşulu.
                if((delimiters[j] == line[i]) && (6 > j && j >= 0))
                {
                    // delimiter karakter gelince token içerisindeki değerin ne olduğunun kontrol edilmesini sağlayan if koşulu
                    if(!controlInteger(token,tokenSize,lex) && tokenSize != 0)
                    {
                        writeIdentifier(token,tokenSize,lex);
                    }

                    // token char dizisinin boşaltılması
                    resetToken(token, tokenSize);
                    tokenSize=0;

                    // commentin içerisinden çıktığımız durum için if koşulu
                    if(line[i] == '*' && line[i+1] == ')')
                    {
                        commentFlag = false;
                        line[i] = '\0';
                        resetToken(token, tokenSize);
                        tokenSize=0;
                        break;
                    }

                    // asıl operatörlerin kesin kararlaştırılma işleminin yapıldığı for döngüsü
                    for(int k = 0; k<6; k++)
                    {
                        //|| ((line[i] == '/')&&(line[i+1] == '')) || ((line[i] == '')&&(line[i+1] == '/'))
                        // operatörün double operatör olup-olmadığının kontrolünün yapılması
                        if(((delimiters[k] == line[i])&&(delimiters[k] == line[i+1])) || ((line[i] == ':')&&(line[i+1] == '=')))
                        {
                            fprintf(lex, "%s(%c%c)\n", "Operator", line[i], line[i+1]); // "Operator" tokenın yazdırılması
                            operatorFlag = true;
                            doubleOperatorFlag = true;

                            // token char dizisinin boşaltılması
                            resetToken(token, tokenSize);
                            tokenSize=0;
                            break;
                        }
                        /*
                        else if(((line[i] == '/')&&(line[i+1] == '')) || ((line[i] == '')&&(line[i+1] == '/'))){

                        }*/
                    }

                    // operatör, double operatör değilse if bloğuna girer
                    if(!doubleOperatorFlag)
                    {
                        fprintf(lex, "%s(%c)\n", "Operator", line[i]); // "Operator" tokenın yazdırılması
                        operatorFlag = true;
                        resetToken(token, tokenSize);  // token char dizisinin boşaltılması
                        tokenSize=0;
                        break;
                    }
                }

                // parantez kontrolü için if koşulu.
                else if((delimiters[j] == line[i]) && (12 > j && j >= 6))
                {
                    bracketFlag = true;
                    if(!controlInteger(token,tokenSize,lex) && tokenSize != 0)
                    {
                        writeIdentifier(token,tokenSize,lex);
                        resetToken(token, tokenSize);  // token char dizisinin boşaltılması
                        tokenSize=0;
                    }

                    // switch-case ile karakterin hangi tür parantez olduğunun bulunması
                    switch(j)
                    {
                        case 6:
                            if(delimiters[2] == line[i+1])
                            {
                                commentFlag = true;
                            }
                            else
                            {
                                fprintf(lex,"%s\n","LeftPar");
                            }
                            break;
                        case 7:
                            fprintf(lex,"%s\n","RightPar");
                            break;
                        case 8:
                            fprintf(lex,"%s\n","LeftSquareBracket");
                            break;
                        case 9:
                            fprintf(lex,"%s\n","RightSquareBracket");
                            break;
                        case 10:
                            fprintf(lex,"%s\n","LeftCurlyBracket");
                            break;
                        case 11:
                            fprintf(lex,"%s\n","RightCurlyBracket");
                            break;
                    }

                    resetToken(token, tokenSize);
                    tokenSize=0;
                }

                // space ve tab karakterleri için if koşulu
                else if((delimiters[12] == line[i]) || (delimiters[15] == line[i]) || (delimiters[16] == line[i]) || ((line[i] == '/')&&(line[i+1] == ' ')) || ((line[i] == ' ')&&(line[i+1] == '/')))
                {
                    spaceFlag = true;
                    int control = controlInteger(token,tokenSize,lex);
                    if(!control && tokenSize != 0)
                    {
                        /*
                        if(delimiters[16] == line[i]){
                            fgetc(psi);
                        }*/

                        writeIdentifier(token,tokenSize,lex);
                        resetToken(token, tokenSize);
                        tokenSize=0;

                    }
                    else if (control)
                    {
                        resetToken(token, tokenSize);
                        tokenSize=0;
                        break;
                    }
                }

                // satır sonu elemanı için if koşulu.
                else if(delimiters[13] == line[i])
                {
                    if(!controlInteger(token,tokenSize,lex)&& tokenSize != 0)
                    {
                        writeIdentifier(token,tokenSize,lex);
                        resetToken(token, tokenSize);
                        tokenSize=0;
                    }
                    endOfLine = true;
                    fprintf(lex,"%s\n","EndOfLine");
                    resetToken(token, tokenSize);
                    tokenSize=0;
                    break;
                }
                // delimiter'i kontrol eden for döngüsünün sonu
            }

            // burada kullanılan flaglar yazılan karakterlerin gelmiş olma durumunda token değişkeni içerisine yerleştirmeden
            // döngünün aynı satır üzerindeki bir sonraki karaktere geçebilmemizi sağlayacak olan if blokları
            if(spaceFlag)
            {
                spaceFlag = false;
                continue;
            }
            if(operatorFlag)
            {
                operatorFlag = false;
                continue;
            }
            if(bracketFlag)
            {
                bracketFlag = false;
                continue;
            }
            if(commentFlag)
            {
                continue;
            }
            if(endOfLine)
            {
                continue;
            }

            // karakterin token içine yerleştirilmesi
            token[tokenSize] = line[i];
            tokenSize++;

            // tokenda biriken değerin keyword olup olmadığının kontrolünü yapan if koşulu
            if(controlKeyword(token,lex))
            {
                resetToken(token, tokenSize);
                tokenSize=0;
            }
            // satırın içerisindekileri okumak için oluşturduğumuz for döngüsünü sonu
        }

        // yeni satıra geçmeden önce gereken sıfırlama işlemi
        endOfLine = false;
        resetToken(token, tokenSize);
        tokenSize=0;
        // satırları okuma işlemimizin bittiği bölüm
    }


    // string ve comment için hata kontrolü.
    if( stringFlag == true )
    {
        fprintf(lex,"%s","Error! The string constant didn't terminate.");
    }
    if(commentFlag)
    {
        fprintf(lex,"%s","Error! The comment didn't terminate.");
    }
    fclose(psi);

    printf("%s","The program finished successfully.");
    return 0;
}

void resetToken(char text[], int tokenSize){  // token'ı sıfırlamamızı sağlayan metot.
    for(int i=0; i<tokenSize; i++){
        text[i] = 0;
    }
}

void lower(char text[]) { // büyük harfleri küçük harf yapan metot.

    int lenght = strlen(text);
    for (int i = 0; i < lenght; i++) {
        if (text[i] > 64 && text[i] < 91) {
            text[i] = text[i] + 32;
        }
    }
}

void upper(char text[]) { // küçük harfleri büyük harf yapan metot.

    int lenght = strlen(text);
    for (int i = 0; i < lenght; i++) {
        if (text[i] > 96 && text[i] < 123) {
            text[i] = text[i] - 32;
        }
    }
}

// Keywordleri kontrol ettiğimiz metod
int controlKeyword(char* token, FILE *lex)
{
    // dil'in keywordslerini tutan char arrayi
    char keywords[18][10] ={"break","case","char","const","continue","do","else","enum",
                            "float","for","goto","if","int", "long","record","return","static","while"}  ;

    lower(token);  // tüm harfleri küçük harfe döndüren fonksiyon

    // keywords arrayinin uzunluğu kadar dönen for döngüsü
    for(int i=0;i<18;i++){
        if(strcmp(token,keywords[i]) == 0) // eşitlik kontrolü
        {
            fprintf(lex,"%s(%s)\n","Keyword",token);
            return 1;
        }
    }
    return 0;
}

// karakterin integer olup-olmadığını anlamamızı sağlayan metot
int controlInteger(char* token, int tokenSize,FILE *lex)
{
    int counter = 0;
    for(int i = 0; i<tokenSize; i++)
    {
        if((58>(int)token[i]) && ((int)token[i]>47))
        {
            counter++;
        }
    }

    // 10'dan daha uzun bir tam sayı değeri kullanıldığında lexical analyzer'ın hata mesajı vermesini sağlayan if koşulu
    if((counter == tokenSize) && counter > 10)
    {
        fprintf(lex,"%s\n","Size error! Maximum integer size is 10 digits.");
        return 1;
    }
    else if((counter == tokenSize)&& counter !=0)
    {
        fprintf(lex,"%s(","IntConst");
        for(int i = 0 ; i<counter; i++)
        {
            fprintf(lex,"%c",token[i]);
        }
        fprintf(lex,"%s",")\n");

        return 1;
    }
    return 0;
}

// identifierları yazdırmak için kullanılan metot
void writeIdentifier(char* token, int tokenSize,FILE *lex)
{
    int check = 0;

    upper(token);

    for(int x=0; x<tokenSize; x++)
    {
        if(!((91>(int)token[x]) && ((int)token[x]>64)) && !((58>(int)token[x]) && ((int)token[x]>47)) && (int)token[x] != 95)
        {
            check =1 ;
            break;
        }
    }

    // Gerekli hata kontrollerinin yapılmasını sağlayan ve hata olmaması durumunda da identifier'i yazdıran if bloğu
    if(tokenSize > 20)
    {
        //----
        fprintf(lex,"%s\n","Size error! Maximum identifier size is 20 characters.");
    }
    else if (!((91>(int)token[0]) && ((int)token[0]>64)))
    {
        fprintf(lex,"%s\n","Error! Identifiers start with an alphabetic character (a letter).");
    }
    else if(check)
    {
        fprintf(lex,"%s\n","Error! Identifiers are composed of one or more letters, digits or_ (underscore)");
    }
    else
    {
        fprintf(lex,"%s(","Identifier");
        for(int k = 0; k<tokenSize; k++)
        {
            fprintf(lex,"%c",token[k]);
        }
        fprintf(lex,"%s",")\n");
    }
}