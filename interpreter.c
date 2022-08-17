#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h> 
#include <assert.h>

static void _mkdir(const char *dir,char temp_dir[100]) {

  // Burada okunan pmk dosyasındaki directory pathler alınıyor

  // pathlerden ana dizin olanı yoksa ilk önce o oluşturuluyor

  // sonra onun altındaki

  // eğer ana dizin varsa onun altındaki yoksa
  // o oluşturuluyor

  // ikisi de yoksa ikisini de oluşturuyoruz. 

  char tmp[256];
  char *p = NULL;
  size_t len;
  snprintf(tmp, sizeof(tmp),"%s",dir);
  len = strlen(tmp);
  // burada klasor olusturma islemleri basliyor
  if(tmp[len - 1] == '/')
          tmp[len - 1] = 0;
  for(p = tmp + 1; *p; p++)
    if(*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU);
            *p = '/';
    }

        int temp=mkdir(tmp, S_IRWXU);
        chdir(temp_dir);
        if(temp==-1){
          printf("\nFile can not be created ! %s\n",strerror(errno));
          
        }
  // burada da klasorlerimiz nereye oluşturulduysa
  // yani hangi directory'deysek oraya 
    
        
}

void islemler(char chr,FILE* dosya,char keyword [100] ){

  /*make ve go islemlerinin yapilarinin yuzde 90ı benzer oldugu icin
  tek bir fonksiyonda islemleri topladik
   make isleminde curr dir degismeyecek
   olasi bir degisim gozlenmesi halinde sigorta gorevi goren
   bir dizi islem yapiyoruz
   en bastaki curr diri alip en sonda yine curr dirimizi bu 
   dizide tuttugumuz degere esitliyoruz*/
  char temp_dir[100];
  char temp_wcd[100];
  getcwd(temp_wcd, 100);
  strcpy(temp_dir, temp_wcd);

  // keywordden sonraki karakteri okuyoruz
  chr=getc(dosya);
    if(isspace(chr)){
      while(isspace(chr)){
        chr=getc(dosya);
      }
    }
    /* keywordden sonraki karakter bosluk ise bosluk karakteri bitene kadar o karakterleri gececegiz ta ki '<' isaretini gorene kadar    */
    if(chr=='<'){
      // iki isaret arasini tutabilmek icin bir string ve onun buyuklugunu tutmak str_size degiskeni tanimliyoruz
      
      char *str_const_2=(char *) malloc(50);
      int str_size=0;
      chr = getc(dosya);
      while(chr!='>' ){
        // burada yine bosluk gordugumuz anda bosluk bitene kadar olan
        // bosluk karakterlerini atliyoruz.
        if(isspace(chr)){
          while(isspace(chr)){
            chr=getc(dosya);
          }
        }
        if(chr=='*'){
          // yildiz isaretini okuyunca hemen bir ust dizine cikiliyor
          // make ile curr dir degismeyecek korkmamiza gerek yoksa
          // yine en sonda ilk basta tuttugumuz ilk curr dire esitleyip cikacagiz
          chdir("..");
          chr=getc(dosya);
          // bosluklari tekrar atliyoruz.
          while(isspace(chr)){
            chr=getc(dosya);
          }
          // / isareti kontrolu
          // eger isaretten sonra harf rakam gelirse hata verip cikacagiz
          if(chr=='/' ){
            chr=getc(dosya);
            // bosluklari yine geciyoruz.
            if(isspace(chr)){
              while(isspace(chr)){
                chr=getc(dosya);
              }
            }
          }

          // eger / isareti olmayıp da harf ve rakam gelirse yine hata verecegiz.
          else if(isalnum(chr)){
            printf("Operator * can only be used at the beginning of path expressions!");
            exit(EXIT_FAILURE);
          }
        }
        // "/"" kontrolu
        else if(chr=='/'){
          // bu isaretten sonrasını string dizimizin icine atliyoruz
          // cunku bu isaretten itibaren olan seyler klasor ismi olacaktir
          str_const_2[str_size]=chr;
          str_size++;
          chr=getc(dosya);
          // boslukları gec
          if(isspace(chr)){
            while(isspace(chr)){
              chr=getc(dosya);
            }
          }
          // hata kontrolu

          // eger onunde bir harf rakam ya da * isareti yoksa tek basina hata verir.
          if(isalnum(chr) || chr==95 || chr==42){
            printf("Operator / cannot be used at the beginning or the end of any path !\n");
            exit(EXIT_FAILURE);
          }
          if(chr=='/'){
              printf("Operator / can not be used back to back! \n");
              exit(EXIT_FAILURE);
          }
        }

        // klasır isimlerini almaya geliyoruz

        if(isalnum(chr) || chr==95){
          str_const_2[str_size]=chr;
          str_size++;
          chr=getc(dosya);
          if(isspace(chr)){
            while(isspace(chr)){
              chr=getc(dosya);
              
        /* ----- BURASI COK ONEMLI------
        PROJE ACIKLAMASINDA BIZDEN KLASOR İSİMLERİ ARASINDA bosluk
        OLMAYACAGINI SOYLUYOR VE BİZ DE BURDA BUNU KONTROL EDİYORUZ*/

              if(isalnum(chr)){
                printf(" Blank characters must not exist in directory names !\n");
                exit(EXIT_FAILURE);
              }
              // string icine karakter atmaya devam
              if(chr=='/'){
                str_const_2[str_size]=chr;
                str_size++;
                chr=getc(dosya);
                if(isspace(chr)){
                  while(isspace(chr)){
                    chr=getc(dosya);
                  }
                }

                // yildiz isareti sadece baslarda olur 
                // 
                if(chr=='*'){
                    printf("Operator * can only be used at the beginning of path expressions !");
                    exit(EXIT_FAILURE);
                }

                // bundan onceki karakter / isareti
                // bundan sonraki karakter yine o isaretse
                // hata verecegiz
                if(chr=='/'){
                  printf("Operator / can not be used back to back!\n");
                  exit(EXIT_FAILURE);
                }
              }
            }
          }

          // dir path almaya devam ediyoruz
          if(chr=='/'){
            str_const_2[str_size]=chr;
            str_size++;
            chr=getc(dosya);
            // boslukları gec
            if(isspace(chr)){
              while(isspace(chr)){
                chr=getc(dosya);
              }
            }
          }
          if(chr=='*'){
            printf("Operator * can only be used at the beginning of path expressions !\n");
            exit(EXIT_FAILURE);
          }
        }
      }
      /*  BURAYA KADAR OLAN İSLEMLAR MAKE VE GO İCİN ORTAKTI
      PARAMETRE OLARAK ALDIGIMIZ KEYWORDLER ile burdan ARTIK YOLLARI AYIRIYORUZ
      KEYWORD MAKE GELİRSE         */
      if(strcmp(keyword, "make")==0){

        // ">" İSARETİNDEN SONRA noktalı virgül gelmeli ki command okunabilsin
        // bunun kontrolu yapiliyor asagida
        chr=getc(dosya);
      char a[100];
      if(chr==';'){
        // gereksiz belllek kullanımını kısıtlamak icin realloc yapiliyor.
        str_const_2 = (char *) realloc(str_const_2, str_size);
        // stringin sonunda / isareti varsa kabul etmiyoruz.
        if(str_const_2[str_size-1]=='/'){
          printf("Operator / cannot be used at the beginning or the end of any path !\n");
          exit(EXIT_FAILURE);
        }
        _mkdir(str_const_2,temp_dir);
      }
      else{
        printf("End of line character required!");
        exit(EXIT_FAILURE);
      }
      }

      // EGER KEYWORD GO İSE islemler asagida
      if(strcmp(keyword, "go")==0){
        chr=getc(dosya);
        if(chr!=';'){
          printf("End of line character required !");
          exit(EXIT_FAILURE);
        }
        // getwcd islemi icin a dizisi tanimlandi
        char a[100];
        if(chr==';'){
          getcwd(a, 100);
          str_const_2[str_size]='\0';
          // stringin sonununu temizledik
          // mevcut konumun sonuna '/' isareti ekledik
          // bunun sebebi pmk dosyasindan okunan yerin path olarak yazilabilmesi
          strcat(a, "/");
          // yeni halin yanına okunan stringi koyuyoruz.
          strcat(a, str_const_2);
          // ve change dir yapıyoruz
          int result;
          result=chdir(a);
          if(result==-1){
            perror("Directory could not be cahnged ! :\n ");
          }
        }

        // asagida da benzer hata islemleri var
        // eger go ile gidecegimiz bir yer yoksa hata verecek
        DIR *dir;
        dir = opendir(a);
        if (dir == NULL) {
          // go ile gidilecek bir yer olmadigi icin ilk yerimize geri donuyoruz
          printf("Could not reach wanted folder !\n");
          chdir(temp_dir);
        }
      }
    }
}



static void if_if_not(char chr,FILE* dosya,char word[100]){

  // burada da islemler fonksiyonuna cok benzer algoritmalar ve islemler bulunuyor
  // farkları ise fonksiyonun sonlarinda gorulecektir

  char temp_dir[100];
  char temp_wcd[100];
  getcwd(temp_wcd, 100);
  strcpy(temp_dir, temp_wcd);
  // burda yine tekrar eski directorynin oldugu yere gidilmesi gerekiyorsa
  // onun icin bir temp curr dir stringi
  char word_2[30];
  chr=getc(dosya);
  /* keywordden sonraki karakter bosluk ise bosluk karakteri bitene kadar o karakterleri gececegiz ta ki '<' isaretini gorene kadar    */
          if(isspace(chr)){
            while(isspace(chr)){
              chr=getc(dosya);
            }
          }
          if(chr=='<'){
            // iki isaret arasini tutabilmek icin 
            // bir string ve onun buyuklugunu tutmak str_size degiskeni tanimliyoruz
            char *str_const_2=(char *) malloc(50);
            int str_size=0;
            chr = getc(dosya);
  // pathlerin yazildigi kisim kapanana kadar bu dongunun icinde kalacagiz.
            while(chr!='>' ){
              if(isspace(chr)){
                while(isspace(chr)){
                  chr=getc(dosya);
                }
              }
              // yildiz kontrolu
              if(chr=='*'){
                 // yildiz isaretini okuyunca hemen bir ust dizine cikiliyor
                 // ilerde degisebilir bu simdilik
                chdir("..");
                chr=getc(dosya);
                // bosluklari gec
                while(isspace(chr)){
                  chr=getc(dosya);
                }
                if(chr=='/' ){
                  chr=getc(dosya);
                  if(isspace(chr)){
                    while(isspace(chr)){
                      chr=getc(dosya);
                    }
                  }
                }

                // harf veya sayi gelemez
                else if(isalnum(chr)){
                  printf("Operator * can only be used at the beginning of path expressions!");
                  exit(EXIT_FAILURE);
                }
              }

              // '/' kontrolu
              else if(chr=='/'){
                str_const_2[str_size]=chr;
                str_size++;
                chr=getc(dosya);
                if(isspace(chr)){
                  while(isspace(chr)){
                    chr=getc(dosya);
                  }
                }
                if(isalnum(chr) || chr==95 || chr==42){
                  printf("Operator / cannot be used at the beginning or the end of any path !\n");
                  exit(EXIT_FAILURE);
                }
                if(chr=='/'){
                    printf("Operator / can not be used abck to back !");
                    exit(EXIT_FAILURE);
                }
              }
              // alınan karakter harf sayı veya _ ise
              if(isalnum(chr) || chr==95){
                str_const_2[str_size]=chr;
                str_size++;
                chr=getc(dosya);
                if(isspace(chr)){
                  while(isspace(chr)){
                    chr=getc(dosya);
                    if(isalnum(chr)){
                      printf("Blank characters must not be in directory names !");
                      exit(EXIT_FAILURE);
                    }
                    // klasor adı stringine atıyoruz yine
                    if(chr=='/'){
                      str_const_2[str_size]=chr;
                      str_size++;
                      chr=getc(dosya);
                      if(isspace(chr)){
                        while(isspace(chr)){
                          chr=getc(dosya);
                        }
                      }
                      if(chr=='*'){
                          printf("Operator * can only be used at the beginning of path expressions !\n");
                          exit(EXIT_FAILURE);
                      }
                      if(chr=='/'){
                        printf("Operator / can not be used abck to back !");
                        exit(EXIT_FAILURE);
                      }
                    }
                  }
                }

                // kelimeden sonra bu isaret geliyorsa bunu da path icin 
                // stringin icine atiyoruz
                if(chr=='/'){
                  str_const_2[str_size]=chr;
                  str_size++;
                  chr=getc(dosya);
                  if(isspace(chr)){
                    while(isspace(chr)){
                      chr=getc(dosya);
                    }
                  }
                }
                // kelimeler arasinda yildiz isareti kontrolu
                if(chr=='*'){
                  printf("Operator * can only be used at the beginning of path expressions !\n");
                  exit(EXIT_FAILURE);
                }
              }
            }
            char s[100];
            getcwd(s,100);
            // şu an bulunduğumuz directorynin yanina istenilen
            // klasör eklemek icic string duzenlemesi
            strcat(s,"/");
            strcat(s,str_const_2);
            int len;
            len=strlen(s);
            s[len]='\0';
            DIR *dir;
            dir = opendir(s);

            
            if(strcmp(word, "if")){
              if (dir != NULL) {
              /* eger klasor acilamazsa yani oyle bir klasor yoksa
              if blogu calismayacak.
              Calısmayacağı için if blogu icindeki aksiyonlar da yerine getirilemeyecektir
              bu yüzden içindeki butun karakterleri atlıyoruz yani gormezlikten geliyoruz.*/
                
                chr=getc(dosya);
                // boslukları yine geciyoruz
                if(isspace(chr)){
                  while(isspace(chr)){
                    chr=getc(dosya);
                  }
                }
                // eger '{' isareti olan bir if ise degil ise tek komutluk olacaktir
                // bu yuzden ; virgul isaretine kadar yok saymamiz gerekecektir.
                if(chr!='{'){
                  while(chr!=';'){
                    chr=getc(dosya);
                  }
                }

                // eger '{' isaretiyle baslayan bir if blogu ise
                // } isaretini gorene kadar yok saymamız gerekiyor.
                while(chr=='{'){
                  while(chr!= '}'){
                    chr=getc(dosya);
                  }
                }
              } 
              else {
                // burada da eger if blogu icindeki path dogru bir deger
                // donduruyorsa burda artık if blogunun icinde hangi komutlar
                // var ona bakılacak
              
              chdir(temp_dir);
              // burada fonksiyona başladığımız yerdeki klasöre tekrar geçis yapıyoruz
              chr=getc(dosya);
              if(isspace(chr)){
                while(isspace(chr)){
                  chr=getc(dosya);
                }
              }
              if(chr!='{'){
                // eger if in cesidi tek komutluk ise { isareti degil ; isareti olacaktir
                // bu yüzden onun kontrolu yapılıyor
                // noktalı virgüle kadar gidilecek ve o aradaki islemler okununp
                // tekrar yapılacak
                if(chr!=';'){
                  if(isspace(chr)){
                    chr = getc(dosya);
                  }
                  // ic ice if bloklarının calısmasi icin asagıdaki gibi 
                  // recursive bir yapı oluşturuluyor
                  if(isalpha(chr)){
                    int word_len_2=0;
                    while(isalpha(chr) || isdigit(chr) || chr == 95){
                      if(isupper(chr)) chr = tolower(chr);
                      word_2[word_len_2++] = chr;
                      chr = getc(dosya);
                    }
                    word_2[word_len_2] = '\0';
                    printf("Keyword: %s \n",word);
                    if(strcmp(word_2, "ifnot")==0){
                      if_if_not(chr, dosya,word_2);
                    }
                    if(strcmp(word_2, "if")==0){
                      if_if_not(chr, dosya,word_2);
                    }
                    if(strcmp(word_2, "make")==0){
                      islemler(chr, dosya, word_2);
                    }
                    if(strcmp(word_2, "go")==0){
                      islemler(chr, dosya, word_2);
                    }
                  }
                }
              }
              // eger if blogu { iceren türden ise ona göre kontrol ediyoruz } isareti olana kadar olan
              // islemleri icindeki okumalardan sonra tekrar recursive islemler yapılıyor.
                while(chr=='{'){
                  chr=getc(dosya);
                    if(chr!='}'){
                    if(isspace(chr)){
                      chr = getc(dosya);
                    }
                    if(isalpha(chr)){
                      int word_len_2=0;
                      while(isalpha(chr) || isdigit(chr) || chr == 95){
                        if(isupper(chr)) chr = tolower(chr);
                        word_2[word_len_2++] = chr;
                        chr = getc(dosya);
                      }
                      word_2[word_len_2] = '\0';
                      if(strcmp(word_2, "ifnot")==0){
                        if_if_not(chr, dosya,word_2);
                      }
                      if(strcmp(word_2, "if")==0){
                        if_if_not(chr, dosya,word_2);
                      }
                      if(strcmp(word_2, "make")==0){
                        islemler(chr, dosya, word_2);
                      }
                      if(strcmp(word_2, "go")==0){
                        islemler(chr, dosya, word_2);
                      }
                    }
                  }
                }
              }
            }
            if(strcmp(word,"ifnot")){
              if (dir == NULL) {
              /* eger klasor acilamazsa yani oyle bir klasor yoksa
              ifnot blogu calisacak.
              Ifnot blogu if blogunun tam tersi islem yapiyo
              Bu yuzden ifnot blogu icindeki deger null bir degere esit degilse icindeki degeri calistirmayacaktir.*/
                
                chr=getc(dosya);
                // boslukları yine geciyoruz
                if(isspace(chr)){
                  while(isspace(chr)){
                    chr=getc(dosya);
                  }
                }
                // eger '{' isareti olan bir if ise degil ise tek komutluk olacaktir
                // bu yuzden ; virgul isaretine kadar yok saymamiz gerekecektir.
                if(chr!='{'){
                  while(chr!=';'){
                    chr=getc(dosya);
                  }
                }

                // eger '{' isaretiyle baslayan bir if blogu ise
                // } isaretini gorene kadar yok saymamız gerekiyor.
                while(chr=='{'){
                  while(chr!= '}'){
                    chr=getc(dosya);
                  }
                }
              } 
              else {
                // bu blokta ise yukardaki islemin tam tersi
              
              chdir(temp_dir);
              chr=getc(dosya);
              if(isspace(chr)){
                while(isspace(chr)){
                  chr=getc(dosya);
                }
              }
              if(chr!='{'){
                if(chr!=';'){
                  if(isspace(chr)){
                    chr = getc(dosya);
                  }
                  if(isalpha(chr)){
                    int word_len_2=0;
                    while(isalpha(chr) || isdigit(chr) || chr == 95){
                      if(isupper(chr)) chr = tolower(chr);
                      word_2[word_len_2++] = chr;
                      chr = getc(dosya);
                    }
                    word_2[word_len_2] = '\0';
                    if(strcmp(word_2, "ifnot")==0){
                      if_if_not(chr, dosya,word_2);
                    }
                    if(strcmp(word_2, "if")==0){
                      if_if_not(chr, dosya,word_2);
                    }
                    if(strcmp(word_2, "make")==0){
                      islemler(chr, dosya, word_2);
                    }
                    if(strcmp(word_2, "go")==0){
                      islemler(chr, dosya, word_2);
                    }
                  }
                }
              }
                while(chr=='{'){
                  chr=getc(dosya);
                    if(chr!='}'){
                    if(isspace(chr)){
                      chr = getc(dosya);
                    }
                    if(isalpha(chr)){
                      int word_len_2=0;
                      while(isalpha(chr) || isdigit(chr) || chr == 95){
                        if(isupper(chr)) chr = tolower(chr);
                        word_2[word_len_2++] = chr;
                        chr = getc(dosya);
                      }
                      // ic ice if bloklarının calısmasi icin asagıdaki gibi 
                      // recursive bir yapı oluşturuluyor
                      word_2[word_len_2] = '\0';
                      printf("Keyword: %s \n",word);
                      if(strcmp(word_2, "ifnot")==0){
                        if_if_not(chr, dosya,word_2);
                      }
                      if(strcmp(word_2, "if")==0){
                        if_if_not(chr, dosya,word_2);
                      }
                      if(strcmp(word_2, "make")==0){
                        islemler(chr, dosya, word_2);
                      }
                      if(strcmp(word_2, "go")==0){
                        islemler(chr, dosya, word_2);
                      }
                    }
                  }
                }
              }
            }
            
          }
}


int main(void) {
  
  char target_file[30];
  char temp_current[100];
  char word_2[30];
  char word[30];
  printf("Enter the directory name: ");
  scanf("%s",target_file);
  // strcat iki tane stringi birlestiriyor
  strcat(target_file, ".pmk");
  FILE *dosya;
  dosya=fopen(target_file, "r");
  char s[100];
  if(dosya!=NULL){
    char chr = getc(dosya);
    while(!feof(dosya)){
      // burada yine boslukları geciyoruz
      if(isspace(chr)){
        chr = getc(dosya);
      }
      
      // burada da komutları alacağız.
      // komutlarımız if ifnot make ve go'dan oluşuyor.
      
      if(isalpha(chr)){
        int word_len=0;
        while(isalpha(chr) || isdigit(chr) || chr == 95){
          if(isupper(chr)) chr = tolower(chr);
          word[word_len++] = chr;
          chr = getc(dosya);
          
        }

        word[word_len] = '\0';
        // ne okudugumuzu gostermek amacıyla print koyuyoruz
        printf("%s \n",word);

        if(strcmp(word, "ifnot")==0){
          if_if_not(chr, dosya,word);
        }
        if(strcmp(word, "if")==0){
          if_if_not(chr, dosya,word);
        }
        
        if(strcmp(word, "make")==0){
          islemler(chr, dosya, word);
        }
        if(strcmp(word, "go")==0){
          islemler(chr, dosya, word);
        }

      }
      chr=getc(dosya);
    }
  }
}
