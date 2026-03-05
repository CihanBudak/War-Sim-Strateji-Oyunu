#define _CRT_SECURE_NO_WARNINGS
#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <curl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define unit "C:\\Users\\grkno\\OneDrive\\belgeler\\Files\\json\\unit_types.json"
#define heroes "C:\\Users\\grkno\\OneDrive\\belgeler\\Files\\json\\heroes.json"
#define creatures "C:\\Users\\grkno\\OneDrive\\belgeler\\Files\\json\\creatures.json"
#define research "C:\\Users\\grkno\\OneDrive\\belgeler\\Files\\json\\research.json"
#define senaryo "C:\\Users\\grkno\\OneDrive\\belgeler\\Files\\json\\indirilen.json" 
#define savas_sim "C:\\Users\\grkno\\OneDrive\\belgeler\\Files\\savas_sim.txt"

#define okcu "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\archer.png"
#define piyade "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\Infantry.png"
#define suvari "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\knight walk 1.png"
#define kusatma "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\pngwing.com.png"

#define ork "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\pngwing.com (2).png"
#define mizrakci "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\pngwing.com (1).png"
#define varg "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\kisspng-russian-armed-forces-bear-military-exercise-5b1135ca75aec6.635609131527854538482.png"
#define troll "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\troll_1.png"

#define roboto "C:\\Users\\grkno\\OneDrive\\Belgeler\\Files\\roboto\\Roboto-Regular.ttf"

struct ozellikler {

	double saldiri;
	double savunma;
	double saglik;
	double kritik_sans;

	char bonus_turu[30];
	double bonus_degeri;

	char etki_turu[30];
	double etki_degeri;

	double arastirma_degeri;

	char aciklama[100];

	double birim_adet;
	double birim_sayisi[5];
	char senaryo_satiri[5][100];
	int senaryo_satiri_kontrol[3];

	double toplam_saldiri;
	double toplam_savunma;
	double toplam_saglik;

	double toplam_yenilen;
};

struct seviyeler {

	struct ozellikler seviye_1;
	struct ozellikler seviye_2;
	struct ozellikler seviye_3;
};

struct arastirmalar {

	struct seviyeler savunma_ustaligi;
	struct seviyeler saldiri_gelistirmesi;
	struct seviyeler elit_egitim;
	struct seviyeler kusatma_ustaligi;
};

struct insan_imparatorlugu {

	struct ozellikler piyadeler;
	struct ozellikler okcular;
	struct ozellikler suvariler;
	struct ozellikler kusatma_makineleri;

	struct ozellikler Alparslan;
	struct ozellikler Fatih_Sultan_Mehmet;
	struct ozellikler Mete_Han;
	struct ozellikler Yavuz_Sultan_Selim;
	struct ozellikler Tugrul_Bey;

	struct ozellikler Ejderha;
	struct ozellikler Agri_Dagi_Devleri;
	struct ozellikler Tepegoz;
	struct ozellikler Karakurt;
	struct ozellikler Samur;
};

struct ork_legi {

	struct ozellikler ork_dovusculeri;
	struct ozellikler mizrakcilar;
	struct ozellikler varg_binicileri;
	struct ozellikler troller;

	struct ozellikler Goruk_Vahsi;
	struct ozellikler Thruk_Kemikkiran;
	struct ozellikler Vrog_Kafakiran;
	struct ozellikler Ugar_Zalim;

	struct ozellikler Kara_Troll;
	struct ozellikler Golge_Kurtlari;
	struct ozellikler Camur_Devleri;
	struct ozellikler Ates_Iblisi;
	struct ozellikler Makrog_Savas_Beyi;
	struct ozellikler Buz_Devleri;
};

struct insan_imparatorlugu insanlar;
struct ork_legi orklar;
struct arastirmalar arastirma;

struct ozellikler research_oku(FILE* ptr, char* birim, char* tur) {

	struct ozellikler donus;

	char tum_birimler[10][30] = { "piyadeler","okcular","suvariler","kusatma_makineleri","ork_dovusculeri","mizrakcilar","varg_binicileri","troller","tum_birimler" };

	char satir[100];
	char tampon[50];
	char gecici[50];

	double sayi;

	rewind(ptr);

	while (fgets(satir, 100, ptr) != NULL) {

		if (strstr(satir, birim) != NULL) {

			for (int i = 0;i < 12;i++) {

				fgets(satir, 100, ptr);

				if (strstr(satir, tur) != NULL) {

					for (int j = 0;j < 2;j++) {

						fgets(satir, 100, ptr);

						if (strstr(satir, "deger") != NULL) {

							sscanf(satir, "%s %s", tampon, gecici);

							strncpy(tampon, gecici + 1, strlen(gecici) - 2);

							tampon[strlen(gecici) - 2] = '\0';

							sayi = atof(tampon);

							donus.arastirma_degeri = sayi;
						}

						else if (strstr(satir, "aciklama") != NULL) {

							for (int i = 0;i < 9;i++) {

								if (strstr(satir, tum_birimler[i]))
									strcpy(donus.aciklama, tum_birimler[i]);
							}
						}
					}
				}
			}
		}
	}

	return donus;
};

double unit_oku(FILE* ptr, char* birim, char* tur) {

	char satir[100];
	char tampon[50];

	double sayi;

	rewind(ptr);

	while (fgets(satir, 100, ptr) != NULL) {

		if (strstr(satir, birim) != NULL) {

			for (int i = 0;i < 4;i++) {

				fgets(satir, 100, ptr);

				if (strstr(satir, tur) != NULL) {

					sscanf(satir, "%s %lf", tampon, &sayi);
				}
			}
		}
	}

	return sayi;
}

struct ozellikler heroes_oku(FILE* ptr, char* birim) {

	struct ozellikler donus;

	char tum_birimler[10][30] = { "piyadeler","okcular","suvariler","kusatma_makineleri","ork_dovusculeri","mizrakcilar","varg_binicileri","troller" };

	char satir[100];
	char tampon[50];
	char gecici[50];

	double sayi;

	rewind(ptr);

	while (fgets(satir, 100, ptr) != NULL) {

		if (strstr(satir, birim) != NULL) {

			for (int i = 0;i < 3;i++) {

				fgets(satir, 100, ptr);

				if (strstr(satir, "bonus_turu") != NULL) {

					sscanf(satir, "%s %s", tampon, gecici);

					strcpy(donus.bonus_turu, gecici);
				}

				else if (strstr(satir, "bonus_degeri") != NULL) {

					sscanf(satir, "%s %s", tampon, gecici);

					strncpy(tampon, gecici + 1, strlen(gecici) - 2);
					tampon[strlen(gecici) - 2] = '\0';

					sayi = atof(tampon);

					donus.bonus_degeri = sayi;
				}

				else if (strstr(satir, "aciklama") != NULL) {

					for (int i = 0;i < 8;i++) {

						if (strstr(satir, tum_birimler[i]))
							strcpy(donus.aciklama, tum_birimler[i]);
					}
				}
			}
		}
	}

	return donus;
}

struct ozellikler creatures_oku(FILE* ptr, char* birim) {

	struct ozellikler donus;

	char tum_birimler[10][30] = { "piyadeler","okcular","suvariler","kusatma_makineleri","ork_dovusculeri","mizrakcilar","varg_binicileri","troller" };

	char satir[100];
	char tampon[50];
	char gecici[50];

	double sayi;

	rewind(ptr);

	while (fgets(satir, 100, ptr) != NULL) {

		if (strstr(satir, birim) != NULL) {

			for (int i = 0;i < 3;i++) {

				fgets(satir, 100, ptr);

				if (strstr(satir, "etki_turu") != NULL) {

					sscanf(satir, "%s %s", tampon, gecici);

					strcpy(donus.etki_turu, gecici);
				}

				else if (strstr(satir, "etki_degeri") != NULL) {

					sscanf(satir, "%s %s", tampon, gecici);

					strncpy(tampon, gecici + 1, strlen(gecici) - 2);
					tampon[strlen(gecici) - 2] = '\0';

					sayi = atof(tampon);

					donus.etki_degeri = sayi;
				}

				else if (strstr(satir, "aciklama") != NULL) {

					for (int i = 0;i < 8;i++) {

						if (strstr(satir, tum_birimler[i]))
							strcpy(donus.aciklama, tum_birimler[i]);
					}
				}
			}
		}
	}
	return donus;
}

void research_ata(FILE* ptr, struct ozellikler* oz, char* birim, char* tur) {

	struct ozellikler donus;

	donus = research_oku(ptr, birim, tur);

	oz->arastirma_degeri = donus.arastirma_degeri;

	strcpy(oz->aciklama, donus.aciklama);
}

void unit_ata(FILE* ptr, struct ozellikler* oz, char* birim) {

	oz->saldiri = unit_oku(ptr, birim, "saldiri");
	oz->savunma = unit_oku(ptr, birim, "savunma");
	oz->saglik = unit_oku(ptr, birim, "saglik");
	oz->kritik_sans = unit_oku(ptr, birim, "kritik_sans");
}

void heroes_ata(FILE* ptr, struct ozellikler* oz, char* birim) {

	struct ozellikler donus;

	donus = heroes_oku(ptr, birim);

	strcpy(oz->bonus_turu, donus.bonus_turu);

	strcpy(oz->aciklama, donus.aciklama);

	oz->bonus_degeri = donus.bonus_degeri;
}

void creatures_ata(FILE* ptr, struct ozellikler* oz, char* birim) {

	struct ozellikler donus;

	donus = creatures_oku(ptr, birim);

	strcpy(oz->etki_turu, donus.etki_turu);

	strcpy(oz->aciklama, donus.aciklama);

	oz->etki_degeri = donus.etki_degeri;
}

struct ozellikler senaryo_oku(FILE* ptr, char* taraf, char* dusman, char isim[5][30]) {

	struct ozellikler donus;

	char satir[100];
	char tampon[50];
	char gecici[50];

	donus.senaryo_satiri_kontrol[0] = -1;
	donus.senaryo_satiri_kontrol[1] = -1;
	donus.senaryo_satiri_kontrol[2] = -1;

	double tut;

	memset(donus.birim_sayisi, 0, sizeof(donus.birim_sayisi));

	rewind(ptr);

	while (fgets(satir, 100, ptr) != NULL) {

		if (strstr(satir, taraf) != NULL) {

			while (fgets(satir, 100, ptr) != NULL) {

				if (strstr(satir, dusman) != NULL)
					break;

				for (int i = 0;i < 4;i++) {

					if (strstr(satir, isim[i]) != NULL) {

						sscanf(satir, "%s %lf", tampon, &tut);

						donus.birim_sayisi[i] = tut;
					}
				}

				if (strstr(satir, "kahramanlar") != NULL) {

					strcpy(donus.senaryo_satiri[0], satir);

					donus.senaryo_satiri_kontrol[0] = 1;
				}

				else if (strstr(satir, "canavarlar") != NULL) {

					strcpy(donus.senaryo_satiri[1], satir);

					donus.senaryo_satiri_kontrol[1] = 1;
				}

				else if (strstr(satir, "arastirma_seviyesi") != NULL) {

					fgets(satir, 100, ptr);

					strcpy(donus.senaryo_satiri[2], satir);

					donus.senaryo_satiri_kontrol[2] = 1;
				}
			}
		}
	}

	if (donus.senaryo_satiri_kontrol[0] == -1)
		strcpy(donus.senaryo_satiri[0], "bulunamadi");

	if (donus.senaryo_satiri_kontrol[1] == -1)
		strcpy(donus.senaryo_satiri[1], "bulunamadi");

	if (donus.senaryo_satiri_kontrol[2] == -1)
		strcpy(donus.senaryo_satiri[2], "bulunamadi");

	return donus;
}

void kahraman_hedefi(struct ozellikler* oz, struct ozellikler* k) {

	if (strstr(k->bonus_turu, "saldiri") != NULL)
		oz->saldiri *= 1 + k->bonus_degeri / 100;

	else if (strstr(k->bonus_turu, "savunma") != NULL)
		oz->savunma *= 1 + k->bonus_degeri / 100;

	else if (strstr(k->bonus_turu, "kritik_sans") != NULL)
		oz->kritik_sans *= 1 + k->bonus_degeri / 100;
}

void canavar_hedefi(struct ozellikler* oz, struct ozellikler* k) {

	if (strstr(k->etki_turu, "saldiri") != NULL)
		oz->saldiri *= 1 + k->etki_degeri / 100;

	else if (strstr(k->etki_turu, "savunma") != NULL)
		oz->savunma *= 1 + k->etki_degeri / 100;

	else if (strstr(k->etki_turu, "kritik_sans") != NULL)
		oz->kritik_sans *= 1 + k->etki_degeri / 100;
}

void arastirma_hedefi_insan(struct ozellikler* oz, char* bolum, int m) {

	if (strstr(bolum, "saldiri")) {
		switch (m) {
		case 0:insanlar.piyadeler.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 1:insanlar.okcular.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 2:insanlar.suvariler.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 3:insanlar.kusatma_makineleri.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 4:insanlar.piyadeler.saldiri *= 1 + oz->arastirma_degeri / 100;
			insanlar.okcular.saldiri *= 1 + oz->arastirma_degeri / 100;
			insanlar.suvariler.saldiri *= 1 + oz->arastirma_degeri / 100;
			insanlar.kusatma_makineleri.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		}
	}

	if (strstr(bolum, "savunma")) {
		switch (m) {
		case 0:insanlar.piyadeler.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 1:insanlar.okcular.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 2:insanlar.suvariler.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 3:insanlar.kusatma_makineleri.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 4:insanlar.piyadeler.savunma *= 1 + oz->arastirma_degeri / 100;
			insanlar.okcular.savunma *= 1 + oz->arastirma_degeri / 100;
			insanlar.suvariler.savunma *= 1 + oz->arastirma_degeri / 100;
			insanlar.kusatma_makineleri.savunma *= 1 + oz->arastirma_degeri / 100;break;
		}
	}

	if (strstr(bolum, "kritik_sans")) {
		switch (m) {
		case 0:insanlar.piyadeler.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 1:insanlar.okcular.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 2:insanlar.suvariler.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 3:insanlar.kusatma_makineleri.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 4:insanlar.piyadeler.kritik_sans *= 1 + oz->arastirma_degeri / 100;
			insanlar.okcular.kritik_sans *= 1 + oz->arastirma_degeri / 100;
			insanlar.suvariler.kritik_sans *= 1 + oz->arastirma_degeri / 100;
			insanlar.kusatma_makineleri.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		}
	}
}

void arastirma_hedefi_ork(struct ozellikler* oz, char* bolum, int m) {

	if (strstr(bolum, "saldiri")) {
		switch (m) {
		case 0:orklar.ork_dovusculeri.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 1:orklar.mizrakcilar.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 2:orklar.varg_binicileri.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 3:orklar.troller.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		case 4:orklar.ork_dovusculeri.saldiri *= 1 + oz->arastirma_degeri / 100;
			orklar.mizrakcilar.saldiri *= 1 + oz->arastirma_degeri / 100;
			orklar.varg_binicileri.saldiri *= 1 + oz->arastirma_degeri / 100;
			orklar.troller.saldiri *= 1 + oz->arastirma_degeri / 100;break;
		}
	}

	if (strstr(bolum, "savunma")) {
		switch (m) {
		case 0:orklar.ork_dovusculeri.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 1:orklar.mizrakcilar.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 2:orklar.varg_binicileri.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 3:orklar.troller.savunma *= 1 + oz->arastirma_degeri / 100;break;
		case 4:orklar.ork_dovusculeri.savunma *= 1 + oz->arastirma_degeri / 100;
			orklar.mizrakcilar.savunma *= 1 + oz->arastirma_degeri / 100;
			orklar.varg_binicileri.savunma *= 1 + oz->arastirma_degeri / 100;
			orklar.troller.savunma *= 1 + oz->arastirma_degeri / 100;break;
		}
	}

	if (strstr(bolum, "kritik_sans")) {
		switch (m) {
		case 0:orklar.ork_dovusculeri.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 1:orklar.mizrakcilar.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 2:orklar.varg_binicileri.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 3:orklar.troller.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		case 4:orklar.ork_dovusculeri.kritik_sans *= 1 + oz->arastirma_degeri / 100;
			orklar.mizrakcilar.kritik_sans *= 1 + oz->arastirma_degeri / 100;
			orklar.varg_binicileri.kritik_sans *= 1 + oz->arastirma_degeri / 100;
			orklar.troller.kritik_sans *= 1 + oz->arastirma_degeri / 100;break;
		}
	}
}

size_t write_callback(void* ptr, size_t boyut, size_t eleman, FILE* p) {
	return fwrite(ptr, boyut, eleman, p);
}

int main()
{
	char linkler[10][50] = { "https://yapbenzet.org.tr/1.json", "https://yapbenzet.org.tr/2.json", "https://yapbenzet.org.tr/3.json", "https://yapbenzet.org.tr/4.json","https://yapbenzet.org.tr/5.json",
					"https://yapbenzet.org.tr/6.json","https://yapbenzet.org.tr/7.json","https://yapbenzet.org.tr/8.json","https://yapbenzet.org.tr/9.json","https://yapbenzet.org.tr/10.json", };

	CURL* curl;
	FILE* senaryo_p = NULL;
	CURLcode indir;

	int senaryo_numarasi;

	printf("kac numarali senaryoyu calistirmak istediginizi seciniz (1-10): ");
	scanf("%d", &senaryo_numarasi);

	char* url = linkler[senaryo_numarasi - 1];


	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\cacert.pem");


	if (curl) {

		senaryo_p = fopen(senaryo, "wb");

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, senaryo_p);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		indir = curl_easy_perform(curl);

		fclose(senaryo_p);
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();



	char insan_birimleri[5][30] = { "piyadeler","okcular","suvariler","kusatma_makineleri","tum_birimler" };
	char ork_birimleri[5][30] = { "ork_dovusculeri","mizrakcilar","varg_binicileri","troller","tum_birimler" };

	char insan_kahramanlari[5][30] = { "Alparslan","Fatih_Sultan_Mehmet","Mete_Han","Yavuz_Sultan_Selim","Tugrul_Bey" };
	char insan_canavarlari[5][30] = { "Ejderha","Agri_Dagi_Devleri","Tepegoz","Karakurt","Samur" };

	char ork_kahramanlari[5][30] = { "Goruk_Vahsi","Thruk_Kemikkiran","Vrog_Kafakiran","Ugar_Zalim" };
	char ork_canavarlari[6][30] = { "Kara_Troll","Golge_Kurtlari","Camur_Devleri","Ates_Iblisi","Makrog_Savas_Beyi","Buz_Devleri" };

	char arastirmalar[10][30] = { "savunma_ustaligi","saldiri_gelistirmesi","elit_egitim","kusatma_ustaligi" };

	struct ozellikler cumle;

	double insan_saldiri, insan_savunma, insan_saglik, ork_saldiri, ork_savunma, ork_saglik, net_hasar;
	double saldiri_yorgunluk1, saldiri_yorgunluk2, savunma_yorgunluk1, savunma_yorgunluk2;

	char tampon[50];
	int seviye;
	int s = 0, k = 0, m = 0;
	int kontrol1 = 1, kontrol2 = 1, kontrol3 = 1, kontrol4 = 1, say = 1, yerlestirme = 0, biriktir = 0;
	int sayac = 1;
	double yorgunluk_hesabi = 1;
	int bar_doluluk;



	FILE* p1, * p2, * p3, * p4, * p5;

	senaryo_p = fopen(senaryo, "r");
	p1 = fopen(unit, "r");
	p2 = fopen(heroes, "r");
	p3 = fopen(creatures, "r");
	p4 = fopen(research, "r");
	p5 = fopen(savas_sim, "w");

	if (p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL || p5 == NULL || senaryo_p == NULL) {

		printf("\ndosya acilamadi\n\n");
		return 0;
	}

	else {

		unit_ata(p1, &insanlar.piyadeler, "piyadeler");
		unit_ata(p1, &insanlar.okcular, "okcular");
		unit_ata(p1, &insanlar.suvariler, "suvariler");
		unit_ata(p1, &insanlar.kusatma_makineleri, "kusatma_makineleri");

		unit_ata(p1, &orklar.ork_dovusculeri, "ork_dovusculeri");
		unit_ata(p1, &orklar.mizrakcilar, "mizrakcilar");
		unit_ata(p1, &orklar.varg_binicileri, "varg_binicileri");
		unit_ata(p1, &orklar.troller, "troller");

		printf("\ninsan birimleri ozellikleri\n\n");
		printf("%.2lf %.2lf %.2lf %.2lf\n", insanlar.piyadeler.saldiri, insanlar.piyadeler.savunma, insanlar.piyadeler.saglik, insanlar.piyadeler.kritik_sans);
		printf("%.2lf %.2lf %.2lf %.2lf\n", insanlar.okcular.saldiri, insanlar.okcular.savunma, insanlar.okcular.saglik, insanlar.okcular.kritik_sans);
		printf("%.2lf %.2lf %.2lf %.2lf\n", insanlar.suvariler.saldiri, insanlar.suvariler.savunma, insanlar.suvariler.saglik, insanlar.suvariler.kritik_sans);
		printf("%.2lf %.2lf %.2lf %.2lf\n\n", insanlar.kusatma_makineleri.saldiri, insanlar.kusatma_makineleri.savunma, insanlar.kusatma_makineleri.saglik, insanlar.kusatma_makineleri.kritik_sans);

		printf("ork birimleri ozellikleri\n\n");
		printf("%.2lf %.2lf %.2lf %.2lf\n", orklar.ork_dovusculeri.saldiri, orklar.ork_dovusculeri.savunma, orklar.ork_dovusculeri.saglik, orklar.ork_dovusculeri.kritik_sans);
		printf("%.2lf %.2lf %.2lf %.2lf\n", orklar.mizrakcilar.saldiri, orklar.mizrakcilar.savunma, orklar.mizrakcilar.saglik, orklar.mizrakcilar.kritik_sans);
		printf("%.2lf %.2lf %.2lf %.2lf\n", orklar.varg_binicileri.saldiri, orklar.varg_binicileri.savunma, orklar.varg_binicileri.saglik, orklar.varg_binicileri.kritik_sans);
		printf("%.2lf %.2lf %.2lf %.2lf\n\n", orklar.troller.saldiri, orklar.troller.savunma, orklar.troller.saglik, orklar.troller.kritik_sans);



		heroes_ata(p2, &insanlar.Alparslan, "Alparslan");
		heroes_ata(p2, &insanlar.Fatih_Sultan_Mehmet, "Fatih_Sultan_Mehmet");
		heroes_ata(p2, &insanlar.Mete_Han, "Mete_Han");
		heroes_ata(p2, &insanlar.Yavuz_Sultan_Selim, "Yavuz_Sultan_Selim");
		heroes_ata(p2, &insanlar.Tugrul_Bey, "Tugrul_Bey");

		heroes_ata(p2, &orklar.Goruk_Vahsi, "Goruk_Vahsi");
		heroes_ata(p2, &orklar.Thruk_Kemikkiran, "Thruk_Kemikkiran");
		heroes_ata(p2, &orklar.Vrog_Kafakiran, "Vrog_Kafakiran");
		heroes_ata(p2, &orklar.Ugar_Zalim, "Ugar_Zalim");

		printf("insan kahramanlari ozellikleri\n\n");
		printf("%s %.2lf %s\n", insanlar.Alparslan.bonus_turu, insanlar.Alparslan.bonus_degeri, insanlar.Alparslan.aciklama);
		printf("%s %.2lf %s\n", insanlar.Fatih_Sultan_Mehmet.bonus_turu, insanlar.Fatih_Sultan_Mehmet.bonus_degeri, insanlar.Fatih_Sultan_Mehmet.aciklama);
		printf("%s %.2lf %s\n", insanlar.Mete_Han.bonus_turu, insanlar.Mete_Han.bonus_degeri, insanlar.Mete_Han.aciklama);
		printf("%s %.2lf %s\n", insanlar.Yavuz_Sultan_Selim.bonus_turu, insanlar.Yavuz_Sultan_Selim.bonus_degeri, insanlar.Yavuz_Sultan_Selim.aciklama);
		printf("%s %.2lf %s\n\n", insanlar.Tugrul_Bey.bonus_turu, insanlar.Tugrul_Bey.bonus_degeri, insanlar.Tugrul_Bey.aciklama);

		printf("ork kahramanlari ozellikleri\n\n");
		printf("%s %.2lf %s\n", orklar.Goruk_Vahsi.bonus_turu, orklar.Goruk_Vahsi.bonus_degeri, orklar.Goruk_Vahsi.aciklama);
		printf("%s %.2lf %s\n", orklar.Thruk_Kemikkiran.bonus_turu, orklar.Thruk_Kemikkiran.bonus_degeri, orklar.Thruk_Kemikkiran.aciklama);
		printf("%s %.2lf %s\n", orklar.Vrog_Kafakiran.bonus_turu, orklar.Vrog_Kafakiran.bonus_degeri, orklar.Vrog_Kafakiran.aciklama);
		printf("%s %.2lf %s\n\n", orklar.Ugar_Zalim.bonus_turu, orklar.Ugar_Zalim.bonus_degeri, orklar.Ugar_Zalim.aciklama);



		creatures_ata(p3, &insanlar.Ejderha, "Ejderha");
		creatures_ata(p3, &insanlar.Agri_Dagi_Devleri, "Agri_Dagi_Devleri");
		creatures_ata(p3, &insanlar.Tepegoz, "Tepegoz");
		creatures_ata(p3, &insanlar.Karakurt, "Karakurt");
		creatures_ata(p3, &insanlar.Samur, "Samur");

		creatures_ata(p3, &orklar.Kara_Troll, "Kara_Troll");
		creatures_ata(p3, &orklar.Golge_Kurtlari, "Golge_Kurtlari");
		creatures_ata(p3, &orklar.Camur_Devleri, "Camur_Devleri");
		creatures_ata(p3, &orklar.Ates_Iblisi, "Ates_Iblisi");
		creatures_ata(p3, &orklar.Makrog_Savas_Beyi, "Makrog_Savas_Beyi");
		creatures_ata(p3, &orklar.Buz_Devleri, "Buz_Devleri");

		printf("insan canavarlari ozellikleri\n\n");
		printf("%s %.2lf %s\n", insanlar.Ejderha.etki_turu, insanlar.Ejderha.etki_degeri, insanlar.Ejderha.aciklama);
		printf("%s %.2lf %s\n", insanlar.Agri_Dagi_Devleri.etki_turu, insanlar.Agri_Dagi_Devleri.etki_degeri, insanlar.Agri_Dagi_Devleri.aciklama);
		printf("%s %.2lf %s\n", insanlar.Tepegoz.etki_turu, insanlar.Tepegoz.etki_degeri, insanlar.Tepegoz.aciklama);
		printf("%s %.2lf %s\n", insanlar.Karakurt.etki_turu, insanlar.Karakurt.etki_degeri, insanlar.Karakurt.aciklama);
		printf("%s %.2lf %s\n\n", insanlar.Samur.etki_turu, insanlar.Samur.etki_degeri, insanlar.Samur.aciklama);

		printf("ork canavarlari ozellikleri\n\n");
		printf("%s %.2lf %s\n", orklar.Kara_Troll.etki_turu, orklar.Kara_Troll.etki_degeri, orklar.Kara_Troll.aciklama);
		printf("%s %.2lf %s\n", orklar.Golge_Kurtlari.etki_turu, orklar.Golge_Kurtlari.etki_degeri, orklar.Golge_Kurtlari.aciklama);
		printf("%s %.2lf %s\n", orklar.Camur_Devleri.etki_turu, orklar.Camur_Devleri.etki_degeri, orklar.Camur_Devleri.aciklama);
		printf("%s %.2lf %s\n", orklar.Ates_Iblisi.etki_turu, orklar.Ates_Iblisi.etki_degeri, orklar.Ates_Iblisi.aciklama);
		printf("%s %.2lf %s\n", orklar.Makrog_Savas_Beyi.etki_turu, orklar.Makrog_Savas_Beyi.etki_degeri, orklar.Makrog_Savas_Beyi.aciklama);
		printf("%s %.2lf %s\n\n", orklar.Buz_Devleri.etki_turu, orklar.Buz_Devleri.etki_degeri, orklar.Buz_Devleri.aciklama);



		research_ata(p4, &arastirma.savunma_ustaligi.seviye_1, "savunma_ustaligi", "seviye_1");
		research_ata(p4, &arastirma.savunma_ustaligi.seviye_2, "savunma_ustaligi", "seviye_2");
		research_ata(p4, &arastirma.savunma_ustaligi.seviye_3, "savunma_ustaligi", "seviye_3");

		research_ata(p4, &arastirma.saldiri_gelistirmesi.seviye_1, "saldiri_gelistirmesi", "seviye_1");
		research_ata(p4, &arastirma.saldiri_gelistirmesi.seviye_2, "saldiri_gelistirmesi", "seviye_2");
		research_ata(p4, &arastirma.saldiri_gelistirmesi.seviye_3, "saldiri_gelistirmesi", "seviye_3");

		research_ata(p4, &arastirma.elit_egitim.seviye_1, "elit_egitim", "seviye_1");
		research_ata(p4, &arastirma.elit_egitim.seviye_2, "elit_egitim", "seviye_2");
		research_ata(p4, &arastirma.elit_egitim.seviye_3, "elit_egitim", "seviye_3");

		research_ata(p4, &arastirma.kusatma_ustaligi.seviye_1, "kusatma_ustaligi", "seviye_1");
		research_ata(p4, &arastirma.kusatma_ustaligi.seviye_2, "kusatma_ustaligi", "seviye_2");
		research_ata(p4, &arastirma.kusatma_ustaligi.seviye_3, "kusatma_ustaligi", "seviye_3");

		printf("arastirmalarin ozellikleri\n\n");
		printf("%.2lf %.2lf %.2lf\n", arastirma.savunma_ustaligi.seviye_1.arastirma_degeri, arastirma.savunma_ustaligi.seviye_2.arastirma_degeri, arastirma.savunma_ustaligi.seviye_3.arastirma_degeri);
		printf("%.2lf %.2lf %.2lf\n", arastirma.saldiri_gelistirmesi.seviye_1.arastirma_degeri, arastirma.saldiri_gelistirmesi.seviye_2.arastirma_degeri, arastirma.saldiri_gelistirmesi.seviye_3.arastirma_degeri);
		printf("%.2lf %.2lf %.2lf\n", arastirma.elit_egitim.seviye_1.arastirma_degeri, arastirma.elit_egitim.seviye_2.arastirma_degeri, arastirma.elit_egitim.seviye_3.arastirma_degeri);
		printf("%.2lf %.2lf %.2lf\n\n", arastirma.kusatma_ustaligi.seviye_1.arastirma_degeri, arastirma.kusatma_ustaligi.seviye_2.arastirma_degeri, arastirma.kusatma_ustaligi.seviye_3.arastirma_degeri);

		printf("%s %s %s\n", arastirma.savunma_ustaligi.seviye_1.aciklama, arastirma.savunma_ustaligi.seviye_2.aciklama, arastirma.savunma_ustaligi.seviye_3.aciklama);
		printf("%s %s %s\n", arastirma.saldiri_gelistirmesi.seviye_1.aciklama, arastirma.saldiri_gelistirmesi.seviye_2.aciklama, arastirma.saldiri_gelistirmesi.seviye_3.aciklama);
		printf("%s %s %s\n", arastirma.elit_egitim.seviye_1.aciklama, arastirma.elit_egitim.seviye_2.aciklama, arastirma.elit_egitim.seviye_3.aciklama);
		printf("%s %s %s\n\n", arastirma.kusatma_ustaligi.seviye_1.aciklama, arastirma.kusatma_ustaligi.seviye_2.aciklama, arastirma.kusatma_ustaligi.seviye_3.aciklama);



		cumle = senaryo_oku(senaryo_p, "insan_imparatorlugu", "ork_legi", insan_birimleri);

		insanlar.piyadeler.birim_adet = cumle.birim_sayisi[0];
		insanlar.okcular.birim_adet = cumle.birim_sayisi[1];
		insanlar.suvariler.birim_adet = cumle.birim_sayisi[2];
		insanlar.kusatma_makineleri.birim_adet = cumle.birim_sayisi[3];

		sscanf(cumle.senaryo_satiri[2], "%s %d\n", tampon, &seviye);

		printf("insan textleri\n\n");
		printf("%s\n%s\n%s\n", cumle.senaryo_satiri[0], cumle.senaryo_satiri[1], cumle.senaryo_satiri[2]);



		m = -1;

		for (s = 0;s < 5;s++) {
			if (strstr(cumle.senaryo_satiri[0], insan_kahramanlari[s]) != NULL)
				m = s;
		}

		switch (m) {
		case 0:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Alparslan.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&insanlar.piyadeler, &insanlar.Alparslan);break;
			case 1: kahraman_hedefi(&insanlar.okcular, &insanlar.Alparslan);break;
			case 2: kahraman_hedefi(&insanlar.suvariler, &insanlar.Alparslan);break;
			case 3: kahraman_hedefi(&insanlar.kusatma_makineleri, &insanlar.Alparslan);break;
			}
			break;

		case 1:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Fatih_Sultan_Mehmet.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&insanlar.piyadeler, &insanlar.Fatih_Sultan_Mehmet);break;
			case 1: kahraman_hedefi(&insanlar.okcular, &insanlar.Fatih_Sultan_Mehmet);break;
			case 2: kahraman_hedefi(&insanlar.suvariler, &insanlar.Fatih_Sultan_Mehmet);break;
			case 3: kahraman_hedefi(&insanlar.kusatma_makineleri, &insanlar.Fatih_Sultan_Mehmet);break;
			}
			break;

		case 2:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Mete_Han.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&insanlar.piyadeler, &insanlar.Mete_Han);break;
			case 1: kahraman_hedefi(&insanlar.okcular, &insanlar.Mete_Han);break;
			case 2: kahraman_hedefi(&insanlar.suvariler, &insanlar.Mete_Han);break;
			case 3: kahraman_hedefi(&insanlar.kusatma_makineleri, &insanlar.Mete_Han);break;
			}
			break;

		case 3:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Yavuz_Sultan_Selim.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&insanlar.piyadeler, &insanlar.Yavuz_Sultan_Selim);break;
			case 1: kahraman_hedefi(&insanlar.okcular, &insanlar.Yavuz_Sultan_Selim);break;
			case 2: kahraman_hedefi(&insanlar.suvariler, &insanlar.Yavuz_Sultan_Selim);break;
			case 3: kahraman_hedefi(&insanlar.kusatma_makineleri, &insanlar.Yavuz_Sultan_Selim);break;
			}
			break;

		case 4:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Tugrul_Bey.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&insanlar.piyadeler, &insanlar.Tugrul_Bey);break;
			case 1: kahraman_hedefi(&insanlar.okcular, &insanlar.Tugrul_Bey);break;
			case 2: kahraman_hedefi(&insanlar.suvariler, &insanlar.Tugrul_Bey);break;
			case 3: kahraman_hedefi(&insanlar.kusatma_makineleri, &insanlar.Tugrul_Bey);break;
			}
			break;
		}


		m = -1;

		for (s = 0;s < 5;s++) {
			if (strstr(cumle.senaryo_satiri[1], insan_canavarlari[s]) != NULL)
				m = s;
		}

		switch (m) {
		case 0:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Ejderha.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&insanlar.piyadeler, &insanlar.Ejderha);break;
			case 1: canavar_hedefi(&insanlar.okcular, &insanlar.Ejderha);break;
			case 2: canavar_hedefi(&insanlar.suvariler, &insanlar.Ejderha);break;
			case 3: canavar_hedefi(&insanlar.kusatma_makineleri, &insanlar.Ejderha);break;
			}
			break;

		case 1:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Agri_Dagi_Devleri.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&insanlar.piyadeler, &insanlar.Agri_Dagi_Devleri);break;
			case 1: canavar_hedefi(&insanlar.okcular, &insanlar.Agri_Dagi_Devleri);break;
			case 2: canavar_hedefi(&insanlar.suvariler, &insanlar.Agri_Dagi_Devleri);break;
			case 3: canavar_hedefi(&insanlar.kusatma_makineleri, &insanlar.Agri_Dagi_Devleri);break;
			}
			break;

		case 2:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Tepegoz.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&insanlar.piyadeler, &insanlar.Tepegoz);break;
			case 1: canavar_hedefi(&insanlar.okcular, &insanlar.Tepegoz);break;
			case 2: canavar_hedefi(&insanlar.suvariler, &insanlar.Tepegoz);break;
			case 3: canavar_hedefi(&insanlar.kusatma_makineleri, &insanlar.Tepegoz);break;
			}
			break;

		case 3:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Karakurt.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&insanlar.piyadeler, &insanlar.Karakurt);break;
			case 1: canavar_hedefi(&insanlar.okcular, &insanlar.Karakurt);break;
			case 2: canavar_hedefi(&insanlar.suvariler, &insanlar.Karakurt);break;
			case 3: canavar_hedefi(&insanlar.kusatma_makineleri, &insanlar.Karakurt);break;
			}
			break;

		case 4:
			for (k = 0;k < 4;k++)
				if (strstr(insanlar.Samur.aciklama, insan_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&insanlar.piyadeler, &insanlar.Samur);break;
			case 1: canavar_hedefi(&insanlar.okcular, &insanlar.Samur);break;
			case 2: canavar_hedefi(&insanlar.suvariler, &insanlar.Samur);break;
			case 3: canavar_hedefi(&insanlar.kusatma_makineleri, &insanlar.Samur);break;
			}
			break;
		}

		m = -1;

		for (s = 0;s < 4;s++) {
			if (strstr(cumle.senaryo_satiri[2], arastirmalar[s]) != NULL)
				m = s;
		}

		switch (m) {
		case 0:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.savunma_ustaligi.seviye_1.aciklama, insan_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_insan(&arastirma.savunma_ustaligi.seviye_1, "savunma", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.savunma_ustaligi.seviye_2.aciklama, insan_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_insan(&arastirma.savunma_ustaligi.seviye_2, "savunma", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.savunma_ustaligi.seviye_3.aciklama, insan_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_insan(&arastirma.savunma_ustaligi.seviye_3, "savunma", m);
				}
			break;

		case 1:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.saldiri_gelistirmesi.seviye_1.aciklama, insan_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_insan(&arastirma.saldiri_gelistirmesi.seviye_1, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.saldiri_gelistirmesi.seviye_2.aciklama, insan_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_insan(&arastirma.saldiri_gelistirmesi.seviye_2, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.saldiri_gelistirmesi.seviye_3.aciklama, insan_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_insan(&arastirma.saldiri_gelistirmesi.seviye_3, "saldiri", m);
				}
			break;

		case 2:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.elit_egitim.seviye_1.aciklama, insan_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_insan(&arastirma.elit_egitim.seviye_1, "kritik_sans", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.elit_egitim.seviye_2.aciklama, insan_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_insan(&arastirma.elit_egitim.seviye_2, "kritik_sans", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.elit_egitim.seviye_3.aciklama, insan_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_insan(&arastirma.elit_egitim.seviye_3, "kritik_sans", m);
				}
			break;

		case 3:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.kusatma_ustaligi.seviye_1.aciklama, insan_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_insan(&arastirma.kusatma_ustaligi.seviye_1, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.kusatma_ustaligi.seviye_2.aciklama, insan_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_insan(&arastirma.kusatma_ustaligi.seviye_2, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.kusatma_ustaligi.seviye_3.aciklama, insan_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_insan(&arastirma.kusatma_ustaligi.seviye_3, "saldiri", m);
				}
			break;
		}



		cumle = senaryo_oku(senaryo_p, "ork_legi", "insan_imparatorlugu", ork_birimleri);

		orklar.ork_dovusculeri.birim_adet = cumle.birim_sayisi[0];
		orklar.mizrakcilar.birim_adet = cumle.birim_sayisi[1];
		orklar.varg_binicileri.birim_adet = cumle.birim_sayisi[2];
		orklar.troller.birim_adet = cumle.birim_sayisi[3];

		sscanf(cumle.senaryo_satiri[2], "%s %d", tampon, &seviye);

		printf("ork textleri\n\n");
		printf("%s\n%s\n%s\n", cumle.senaryo_satiri[0], cumle.senaryo_satiri[1], cumle.senaryo_satiri[2]);

		printf("insanlar ve orklarin birim sayilari\n\n");
		printf("%.2lf %.2lf %.2lf %.2lf\n", insanlar.piyadeler.birim_adet, insanlar.okcular.birim_adet, insanlar.suvariler.birim_adet, insanlar.kusatma_makineleri.birim_adet);
		printf("%.2lf %.2lf %.2lf %.2lf\n\n", orklar.ork_dovusculeri.birim_adet, orklar.mizrakcilar.birim_adet, orklar.varg_binicileri.birim_adet, orklar.troller.birim_adet);


		m = -1;

		for (s = 0;s < 4;s++) {
			if (strstr(cumle.senaryo_satiri[0], ork_kahramanlari[s]) != NULL)
				m = s;
		}

		switch (m) {
		case 0:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Goruk_Vahsi.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&orklar.ork_dovusculeri, &orklar.Goruk_Vahsi);break;
			case 1: kahraman_hedefi(&orklar.mizrakcilar, &orklar.Goruk_Vahsi);break;
			case 2: kahraman_hedefi(&orklar.varg_binicileri, &orklar.Goruk_Vahsi);break;
			case 3: kahraman_hedefi(&orklar.troller, &orklar.Goruk_Vahsi);break;
			}
			break;

		case 1:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Thruk_Kemikkiran.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&orklar.ork_dovusculeri, &orklar.Thruk_Kemikkiran);break;
			case 1: kahraman_hedefi(&orklar.mizrakcilar, &orklar.Thruk_Kemikkiran);break;
			case 2: kahraman_hedefi(&orklar.varg_binicileri, &orklar.Thruk_Kemikkiran);break;
			case 3: kahraman_hedefi(&orklar.troller, &orklar.Thruk_Kemikkiran);break;
			}
			break;

		case 2:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Vrog_Kafakiran.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&orklar.ork_dovusculeri, &orklar.Vrog_Kafakiran);break;
			case 1: kahraman_hedefi(&orklar.mizrakcilar, &orklar.Vrog_Kafakiran);break;
			case 2: kahraman_hedefi(&orklar.varg_binicileri, &orklar.Vrog_Kafakiran);break;
			case 3: kahraman_hedefi(&orklar.troller, &orklar.Vrog_Kafakiran);break;
			}
			break;

		case 3:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Ugar_Zalim.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: kahraman_hedefi(&orklar.ork_dovusculeri, &orklar.Ugar_Zalim);break;
			case 1: kahraman_hedefi(&orklar.mizrakcilar, &orklar.Ugar_Zalim);break;
			case 2: kahraman_hedefi(&orklar.varg_binicileri, &orklar.Ugar_Zalim);break;
			case 3: kahraman_hedefi(&orklar.troller, &orklar.Ugar_Zalim);break;
			}
			break;
		}



		m = -1;

		for (s = 0;s < 6;s++) {
			if (strstr(cumle.senaryo_satiri[1], ork_canavarlari[s]) != NULL)
				m = s;
		}

		switch (m) {
		case 0:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Kara_Troll.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&orklar.ork_dovusculeri, &orklar.Kara_Troll);break;
			case 1: canavar_hedefi(&orklar.mizrakcilar, &orklar.Kara_Troll);break;
			case 2: canavar_hedefi(&orklar.varg_binicileri, &orklar.Kara_Troll);break;
			case 3: canavar_hedefi(&orklar.troller, &orklar.Kara_Troll);break;
			}
			break;

		case 1:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Golge_Kurtlari.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&orklar.ork_dovusculeri, &orklar.Golge_Kurtlari);break;
			case 1: canavar_hedefi(&orklar.mizrakcilar, &orklar.Golge_Kurtlari);break;
			case 2: canavar_hedefi(&orklar.varg_binicileri, &orklar.Golge_Kurtlari);break;
			case 3: canavar_hedefi(&orklar.troller, &orklar.Golge_Kurtlari);break;
			}
			break;

		case 2:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Camur_Devleri.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&orklar.ork_dovusculeri, &orklar.Camur_Devleri);break;
			case 1: canavar_hedefi(&orklar.mizrakcilar, &orklar.Camur_Devleri);break;
			case 2: canavar_hedefi(&orklar.varg_binicileri, &orklar.Camur_Devleri);break;
			case 3: canavar_hedefi(&orklar.troller, &orklar.Camur_Devleri);break;
			}
			break;

		case 3:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Ates_Iblisi.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&orklar.ork_dovusculeri, &orklar.Ates_Iblisi);break;
			case 1: canavar_hedefi(&orklar.mizrakcilar, &orklar.Ates_Iblisi);break;
			case 2: canavar_hedefi(&orklar.varg_binicileri, &orklar.Ates_Iblisi);break;
			case 3: canavar_hedefi(&orklar.troller, &orklar.Ates_Iblisi);break;
			}
			break;

		case 4:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Makrog_Savas_Beyi.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&orklar.ork_dovusculeri, &orklar.Makrog_Savas_Beyi);break;
			case 1: canavar_hedefi(&orklar.mizrakcilar, &orklar.Makrog_Savas_Beyi);break;
			case 2: canavar_hedefi(&orklar.varg_binicileri, &orklar.Makrog_Savas_Beyi);break;
			case 3: canavar_hedefi(&orklar.troller, &orklar.Makrog_Savas_Beyi);break;
			}
			break;

		case 5:
			for (k = 0;k < 4;k++)
				if (strstr(orklar.Buz_Devleri.aciklama, ork_birimleri[k]) != NULL)
					m = k;

			switch (m) {
			case 0: canavar_hedefi(&orklar.ork_dovusculeri, &orklar.Buz_Devleri);break;
			case 1: canavar_hedefi(&orklar.mizrakcilar, &orklar.Buz_Devleri);break;
			case 2: canavar_hedefi(&orklar.varg_binicileri, &orklar.Buz_Devleri);break;
			case 3: canavar_hedefi(&orklar.troller, &orklar.Buz_Devleri);break;
			}
			break;
		}



		m = -1;

		for (s = 0;s < 4;s++) {
			if (strstr(cumle.senaryo_satiri[2], arastirmalar[s]) != NULL)
				m = s;
		}

		switch (m) {
		case 0:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.savunma_ustaligi.seviye_1.aciklama, ork_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_ork(&arastirma.savunma_ustaligi.seviye_1, "savunma", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.savunma_ustaligi.seviye_2.aciklama, ork_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_ork(&arastirma.savunma_ustaligi.seviye_2, "savunma", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.savunma_ustaligi.seviye_3.aciklama, ork_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_ork(&arastirma.savunma_ustaligi.seviye_3, "savunma", m);
				}
			break;

		case 1:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.saldiri_gelistirmesi.seviye_1.aciklama, ork_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_ork(&arastirma.saldiri_gelistirmesi.seviye_1, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.saldiri_gelistirmesi.seviye_2.aciklama, ork_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_ork(&arastirma.saldiri_gelistirmesi.seviye_2, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.saldiri_gelistirmesi.seviye_3.aciklama, ork_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_ork(&arastirma.saldiri_gelistirmesi.seviye_3, "saldiri", m);
				}
			break;

		case 2:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.elit_egitim.seviye_1.aciklama, ork_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_ork(&arastirma.elit_egitim.seviye_1, "kritik_sans", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.elit_egitim.seviye_2.aciklama, ork_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_ork(&arastirma.elit_egitim.seviye_2, "kritik_sans", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.elit_egitim.seviye_3.aciklama, ork_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_ork(&arastirma.elit_egitim.seviye_3, "kritik_sans", m);
				}
			break;

		case 3:
			for (k = 0;k < 5;k++)
				if (strstr(arastirma.kusatma_ustaligi.seviye_1.aciklama, ork_birimleri[k]) != NULL && seviye == 1) {
					m = k;
					arastirma_hedefi_ork(&arastirma.kusatma_ustaligi.seviye_1, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.kusatma_ustaligi.seviye_2.aciklama, ork_birimleri[k]) != NULL && seviye == 2) {
					m = k;
					arastirma_hedefi_ork(&arastirma.kusatma_ustaligi.seviye_2, "saldiri", m);
				}

			for (k = 0;k < 5;k++)
				if (strstr(arastirma.kusatma_ustaligi.seviye_3.aciklama, ork_birimleri[k]) != NULL && seviye == 3) {
					m = k;
					arastirma_hedefi_ork(&arastirma.kusatma_ustaligi.seviye_3, "saldiri", m);
				}
			break;
		}



		int sayacc = 1;
		int kenar;

		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_PNG);
		TTF_Init();

		SDL_Rect ekran;
		SDL_GetDisplayUsableBounds(0, &ekran);

		SDL_Window* pencere = SDL_CreateWindow("oyun", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ekran.h - 16, ekran.h - 16, SDL_WINDOW_SHOWN);

		SDL_Renderer* olusturucu = SDL_CreateRenderer(pencere, -1, SDL_RENDERER_ACCELERATED);

		SDL_Event event;

		SDL_Texture* piyade_texture = IMG_LoadTexture(olusturucu, ork);
		SDL_Texture* okcu_texture = IMG_LoadTexture(olusturucu, mizrakci);
		SDL_Texture* suvari_texture = IMG_LoadTexture(olusturucu, varg);
		SDL_Texture* kusatma_texture = IMG_LoadTexture(olusturucu, troll);

		SDL_Texture* ork_texture = IMG_LoadTexture(olusturucu, ork);
		SDL_Texture* mizrakci_texture = IMG_LoadTexture(olusturucu, mizrakci);
		SDL_Texture* varg_texture = IMG_LoadTexture(olusturucu, varg);
		SDL_Texture* troll_texture = IMG_LoadTexture(olusturucu, troll);


		while (sayacc) {
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT)
					sayacc = 0;
			}

			SDL_SetRenderDrawColor(olusturucu, 128, 0, 128, 255);
			SDL_RenderClear(olusturucu);

			kenar = ekran.h / 20;

			for (int i = 0;i < kenar; i++) {

				for (int j = 0;j < kenar;j++) {

					if (i * kenar >= 10 * kenar) {

						SDL_Rect izgara = { i * kenar,j * kenar,kenar,kenar };

						SDL_SetRenderDrawColor(olusturucu, 0, 0, 255, 255);
						SDL_RenderFillRect(olusturucu, &izgara);
					}
				}
			}

			for (int i = 0;i < kenar; i++) {

				for (int j = 0;j < kenar;j++) {

					SDL_SetRenderDrawColor(olusturucu, 0, 0, 0, 255);

					SDL_Rect izgara = { i * kenar,j * kenar,kenar,kenar };
					SDL_RenderDrawRect(olusturucu, &izgara);
				}
			}

			SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

			kontrol1 = 1, kontrol2 = 1, kontrol3 = 1, kontrol4 = 1, say = 1, yerlestirme = 0, biriktir = 0;

			for (int i = 0;i < 10;i++) {
				for (int j = 0;j < 20;j++) {

					if (insanlar.piyadeler.birim_adet > 0 && kontrol1 == 1) {

						yerlestirme = ((int)insanlar.piyadeler.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 5 + 40 * i, 12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(piyade);
						piyade_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 12 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, piyade_texture, NULL, &hedef);

						if (say * 100 >= insanlar.piyadeler.birim_adet) {
							kontrol1 = 0;
							say = 0;
							biriktir = 0;
						}

						say++;
					}

					else if (insanlar.okcular.birim_adet > 0 && kontrol2 == 1) {

						yerlestirme = ((int)insanlar.okcular.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 5 + 40 * i, 12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(okcu);
						okcu_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 12 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, okcu_texture, NULL, &hedef);

						if (say * 100 >= insanlar.okcular.birim_adet) {
							kontrol2 = 0;
							say = 0;
							biriktir = 0;
						}

						say++;
					}

					else if (insanlar.suvariler.birim_adet > 0 && kontrol3 == 1) {

						yerlestirme = ((int)insanlar.suvariler.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 5 + 40 * i, 12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(suvari);
						suvari_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 12 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, suvari_texture, NULL, &hedef);

						if (say * 100 >= insanlar.suvariler.birim_adet) {
							kontrol3 = 0;
							say = 0;
							biriktir = 0;
						}

						say++;
					}

					else if (insanlar.kusatma_makineleri.birim_adet > 0 && kontrol4 == 1) {

						yerlestirme = ((int)insanlar.kusatma_makineleri.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 5 + 40 * i, 12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(kusatma);
						kusatma_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 12 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, kusatma_texture, NULL, &hedef);

						if (say * 100 >= insanlar.kusatma_makineleri.birim_adet)
							kontrol4 = 0;

						say++;
					}
				}
			}

			kontrol1 = 1, kontrol2 = 1, kontrol3 = 1, kontrol4 = 1, say = 1, yerlestirme = 0, biriktir = 0;

			for (int i = 0;i < 10;i++) {
				for (int j = 0;j < 20;j++) {

					if (orklar.ork_dovusculeri.birim_adet > 0 && kontrol1 == 1) {

						yerlestirme = ((int)orklar.ork_dovusculeri.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 405 + 40 * i, 12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(ork);
						ork_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 412 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, ork_texture, NULL, &hedef);

						if (say * 100 >= orklar.ork_dovusculeri.birim_adet) {
							kontrol1 = 0;
							say = 0;
							biriktir = 0;
						}

						say++;
					}

					else if (orklar.mizrakcilar.birim_adet > 0 && kontrol2 == 1) {

						yerlestirme = ((int)orklar.mizrakcilar.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 405 + 40 * i, 12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(mizrakci);
						mizrakci_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 412 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, mizrakci_texture, NULL, &hedef);

						if (say * 100 >= orklar.mizrakcilar.birim_adet) {
							kontrol2 = 0;
							say = 0;
							biriktir = 0;
						}

						say++;
					}

					else if (orklar.varg_binicileri.birim_adet > 0 && kontrol3 == 1) {

						yerlestirme = ((int)orklar.varg_binicileri.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 405 + 40 * i, 12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(varg);
						varg_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 412 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, varg_texture, NULL, &hedef);

						if (say * 100 >= orklar.varg_binicileri.birim_adet) {
							kontrol3 = 0;
							say = 0;
							biriktir = 0;
						}

						say++;
					}

					else if (orklar.troller.birim_adet > 0 && kontrol4 == 1) {

						yerlestirme = ((int)orklar.troller.birim_adet - biriktir) % 100;
						if (yerlestirme == 0)
							yerlestirme = 100;
						biriktir += yerlestirme;

						char birim_adet_str[20];
						sprintf(birim_adet_str, "%d", yerlestirme);

						TTF_Font* font = TTF_OpenFont(roboto, 48);
						SDL_Color color = { 255, 255, 255, 255 };
						SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
						SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
						SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
						SDL_FreeSurface(yuzey_yazi);
						SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);

						SDL_Rect bar = { 405 + 40 * i,12 + 40 * j, 10, 25 };
						SDL_RenderFillRect(olusturucu, &bar);

						SDL_Surface* yuz = IMG_Load(troll);
						troll_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
						SDL_FreeSurface(yuz);

						SDL_Rect hedef;
						hedef.x = 412 + 40 * i;
						hedef.y = 12 + 40 * j;
						hedef.w = 30;
						hedef.h = 30;

						SDL_RenderCopy(olusturucu, troll_texture, NULL, &hedef);

						if (say * 100 >= orklar.troller.birim_adet)
							kontrol4 = 0;

						say++;
					}
				}
			}

			SDL_RenderPresent(olusturucu);
		}



		insanlar.piyadeler.toplam_saglik = insanlar.piyadeler.saglik * insanlar.piyadeler.birim_adet;
		insanlar.okcular.toplam_saglik = insanlar.okcular.saglik * insanlar.okcular.birim_adet;
		insanlar.suvariler.toplam_saglik = insanlar.suvariler.saglik * insanlar.suvariler.birim_adet;
		insanlar.kusatma_makineleri.toplam_saglik = insanlar.kusatma_makineleri.saglik * insanlar.kusatma_makineleri.birim_adet;

		orklar.ork_dovusculeri.toplam_saglik = orklar.ork_dovusculeri.saglik * orklar.ork_dovusculeri.birim_adet;
		orklar.mizrakcilar.toplam_saglik = orklar.mizrakcilar.saglik * orklar.mizrakcilar.birim_adet;
		orklar.varg_binicileri.toplam_saglik = orklar.varg_binicileri.saglik * orklar.varg_binicileri.birim_adet;
		orklar.troller.toplam_saglik = orklar.troller.saglik * orklar.troller.birim_adet;



		insan_saglik = insanlar.piyadeler.toplam_saglik + insanlar.okcular.toplam_saglik +
			insanlar.suvariler.toplam_saglik + insanlar.kusatma_makineleri.toplam_saglik;

		ork_saglik = orklar.ork_dovusculeri.toplam_saglik + orklar.mizrakcilar.toplam_saglik +
			orklar.varg_binicileri.toplam_saglik + orklar.troller.toplam_saglik;



		while (insan_saglik >= 0 && ork_saglik >= 0) {



			insanlar.piyadeler.toplam_saldiri = insanlar.piyadeler.saldiri * insanlar.piyadeler.birim_adet;
			insanlar.okcular.toplam_saldiri = insanlar.okcular.saldiri * insanlar.okcular.birim_adet;
			insanlar.suvariler.toplam_saldiri = insanlar.suvariler.saldiri * insanlar.suvariler.birim_adet;
			insanlar.kusatma_makineleri.toplam_saldiri = insanlar.kusatma_makineleri.saldiri * insanlar.kusatma_makineleri.birim_adet;

			insanlar.piyadeler.toplam_savunma = insanlar.piyadeler.savunma * insanlar.piyadeler.birim_adet;
			insanlar.okcular.toplam_savunma = insanlar.okcular.savunma * insanlar.okcular.birim_adet;
			insanlar.suvariler.toplam_savunma = insanlar.suvariler.savunma * insanlar.suvariler.birim_adet;
			insanlar.kusatma_makineleri.toplam_savunma = insanlar.kusatma_makineleri.savunma * insanlar.kusatma_makineleri.birim_adet;

			orklar.ork_dovusculeri.toplam_saldiri = orklar.ork_dovusculeri.saldiri * orklar.ork_dovusculeri.birim_adet;
			orklar.mizrakcilar.toplam_saldiri = orklar.mizrakcilar.saldiri * orklar.mizrakcilar.birim_adet;
			orklar.varg_binicileri.toplam_saldiri = orklar.varg_binicileri.saldiri * orklar.varg_binicileri.birim_adet;
			orklar.troller.toplam_saldiri = orklar.troller.saldiri * orklar.troller.birim_adet;

			orklar.ork_dovusculeri.toplam_savunma = orklar.ork_dovusculeri.savunma * orklar.ork_dovusculeri.birim_adet;
			orklar.mizrakcilar.toplam_savunma = orklar.mizrakcilar.savunma * orklar.mizrakcilar.birim_adet;
			orklar.varg_binicileri.toplam_savunma = orklar.varg_binicileri.savunma * orklar.varg_binicileri.birim_adet;
			orklar.troller.toplam_savunma = orklar.troller.savunma * orklar.troller.birim_adet;



			if (sayac % 5 == 0 && yorgunluk_hesabi < 10) {

				insanlar.piyadeler.saldiri *= (10 - yorgunluk_hesabi) / 10;
				insanlar.piyadeler.savunma *= (10 - yorgunluk_hesabi) / 10;

				insanlar.okcular.saldiri *= (10 - yorgunluk_hesabi) / 10;
				insanlar.okcular.savunma *= (10 - yorgunluk_hesabi) / 10;

				insanlar.suvariler.saldiri *= (10 - yorgunluk_hesabi) / 10;
				insanlar.suvariler.savunma *= (10 - yorgunluk_hesabi) / 10;

				insanlar.kusatma_makineleri.saldiri *= (10 - yorgunluk_hesabi) / 10;
				insanlar.kusatma_makineleri.savunma *= (10 - yorgunluk_hesabi) / 10;

				orklar.ork_dovusculeri.saldiri *= (10 - yorgunluk_hesabi) / 10;
				orklar.ork_dovusculeri.savunma *= (10 - yorgunluk_hesabi) / 10;

				orklar.mizrakcilar.saldiri *= (10 - yorgunluk_hesabi) / 10;
				orklar.mizrakcilar.savunma *= (10 - yorgunluk_hesabi) / 10;

				orklar.varg_binicileri.saldiri *= (10 - yorgunluk_hesabi) / 10;
				orklar.varg_binicileri.savunma *= (10 - yorgunluk_hesabi) / 10;

				orklar.troller.saldiri *= (10 - yorgunluk_hesabi) / 10;
				orklar.troller.savunma *= (10 - yorgunluk_hesabi) / 10;

				yorgunluk_hesabi++;
			}



			insanlar.piyadeler.toplam_saldiri = insanlar.piyadeler.saldiri * insanlar.piyadeler.birim_adet;
			insanlar.okcular.toplam_saldiri = insanlar.okcular.saldiri * insanlar.okcular.birim_adet;
			insanlar.suvariler.toplam_saldiri = insanlar.suvariler.saldiri * insanlar.suvariler.birim_adet;
			insanlar.kusatma_makineleri.toplam_saldiri = insanlar.kusatma_makineleri.saldiri * insanlar.kusatma_makineleri.birim_adet;

			insanlar.piyadeler.toplam_savunma = insanlar.piyadeler.savunma * insanlar.piyadeler.birim_adet;
			insanlar.okcular.toplam_savunma = insanlar.okcular.savunma * insanlar.okcular.birim_adet;
			insanlar.suvariler.toplam_savunma = insanlar.suvariler.savunma * insanlar.suvariler.birim_adet;
			insanlar.kusatma_makineleri.toplam_savunma = insanlar.kusatma_makineleri.savunma * insanlar.kusatma_makineleri.birim_adet;

			orklar.ork_dovusculeri.toplam_saldiri = orklar.ork_dovusculeri.saldiri * orklar.ork_dovusculeri.birim_adet;
			orklar.mizrakcilar.toplam_saldiri = orklar.mizrakcilar.saldiri * orklar.mizrakcilar.birim_adet;
			orklar.varg_binicileri.toplam_saldiri = orklar.varg_binicileri.saldiri * orklar.varg_binicileri.birim_adet;
			orklar.troller.toplam_saldiri = orklar.troller.saldiri * orklar.troller.birim_adet;

			orklar.ork_dovusculeri.toplam_savunma = orklar.ork_dovusculeri.savunma * orklar.ork_dovusculeri.birim_adet;
			orklar.mizrakcilar.toplam_savunma = orklar.mizrakcilar.savunma * orklar.mizrakcilar.birim_adet;
			orklar.varg_binicileri.toplam_savunma = orklar.varg_binicileri.savunma * orklar.varg_binicileri.birim_adet;
			orklar.troller.toplam_savunma = orklar.troller.savunma * orklar.troller.birim_adet;



			if (insanlar.piyadeler.kritik_sans > 0) {
				if (sayac % (100 / (int)(insanlar.piyadeler.kritik_sans)) == 0) {
					insanlar.piyadeler.toplam_saldiri += insanlar.piyadeler.toplam_saldiri / 2;
				}
			}

			if (insanlar.okcular.kritik_sans > 0) {
				if (sayac % (100 / (int)(insanlar.okcular.kritik_sans)) == 0) {
					insanlar.okcular.toplam_saldiri += insanlar.okcular.toplam_saldiri / 2;
				}
			}

			if (insanlar.suvariler.kritik_sans > 0) {
				if (sayac % (100 / (int)(insanlar.suvariler.kritik_sans)) == 0) {
					insanlar.suvariler.toplam_saldiri += insanlar.suvariler.toplam_saldiri / 2;
				}
			}

			if (insanlar.kusatma_makineleri.kritik_sans > 0) {
				if (sayac % (100 / (int)(insanlar.kusatma_makineleri.kritik_sans)) == 0) {
					insanlar.kusatma_makineleri.toplam_saldiri += insanlar.kusatma_makineleri.toplam_saldiri / 2;
				}
			}

			if (orklar.ork_dovusculeri.kritik_sans > 0) {
				if (sayac % (100 / (int)(orklar.ork_dovusculeri.kritik_sans)) == 0) {
					orklar.ork_dovusculeri.toplam_saldiri += orklar.ork_dovusculeri.toplam_saldiri / 2;
				}
			}

			if (orklar.mizrakcilar.kritik_sans > 0) {
				if (sayac % (100 / (int)(orklar.mizrakcilar.kritik_sans)) == 0) {
					orklar.mizrakcilar.toplam_saldiri += orklar.mizrakcilar.toplam_saldiri / 2;
				}
			}

			if (orklar.varg_binicileri.kritik_sans > 0) {
				if (sayac % (100 / (int)(orklar.varg_binicileri.kritik_sans)) == 0) {
					orklar.varg_binicileri.toplam_saldiri += orklar.varg_binicileri.toplam_saldiri / 2;
				}
			}

			if (orklar.troller.kritik_sans > 0) {
				if (sayac % (100 / (int)(orklar.troller.kritik_sans)) == 0) {
					orklar.troller.toplam_saldiri += orklar.troller.toplam_saldiri / 2;
				}
			}



			insan_saldiri = insanlar.piyadeler.toplam_saldiri + insanlar.okcular.toplam_saldiri +
				insanlar.suvariler.toplam_saldiri + insanlar.kusatma_makineleri.toplam_saldiri;

			ork_saldiri = orklar.ork_dovusculeri.toplam_saldiri + orklar.mizrakcilar.toplam_saldiri +
				orklar.varg_binicileri.toplam_saldiri + orklar.troller.toplam_saldiri;

			insan_savunma = insanlar.piyadeler.toplam_savunma + insanlar.okcular.toplam_savunma +
				insanlar.suvariler.toplam_savunma + insanlar.kusatma_makineleri.toplam_savunma;

			ork_savunma = orklar.ork_dovusculeri.toplam_savunma + orklar.mizrakcilar.toplam_savunma +
				orklar.varg_binicileri.toplam_savunma + orklar.troller.toplam_savunma;



			if (insan_saldiri > ork_savunma) {

				fprintf(p5, "\n******************************\n");
				fprintf(p5, "ADIM %d:insan saldirisi\n", sayac);
				fprintf(p5, "******************************\n\n");

				net_hasar = insan_saldiri - ork_savunma;

				orklar.ork_dovusculeri.toplam_saglik -= (orklar.ork_dovusculeri.toplam_savunma / ork_savunma) * net_hasar;
				orklar.ork_dovusculeri.toplam_yenilen += (orklar.ork_dovusculeri.toplam_savunma / ork_savunma) * net_hasar;

				orklar.mizrakcilar.toplam_saglik -= (orklar.mizrakcilar.toplam_savunma / ork_savunma) * net_hasar;
				orklar.mizrakcilar.toplam_yenilen += (orklar.mizrakcilar.toplam_savunma / ork_savunma) * net_hasar;

				orklar.varg_binicileri.toplam_saglik -= (orklar.varg_binicileri.toplam_savunma / ork_savunma) * net_hasar;
				orklar.varg_binicileri.toplam_yenilen += (orklar.varg_binicileri.toplam_savunma / ork_savunma) * net_hasar;

				orklar.troller.toplam_saglik -= (orklar.troller.toplam_savunma / ork_savunma) * net_hasar;
				orklar.troller.toplam_yenilen += (orklar.troller.toplam_savunma / ork_savunma) * net_hasar;

				ork_saglik -= net_hasar;


				if (insanlar.piyadeler.birim_adet > 0)
					fprintf(p5, "piyadelerin saldiri gucu:%.2lf\n", insanlar.piyadeler.toplam_saldiri);
				if (insanlar.okcular.birim_adet > 0)
					fprintf(p5, "okcularin saldiri gucu:%.2lf\n", insanlar.okcular.toplam_saldiri);
				if (insanlar.suvariler.birim_adet > 0)
					fprintf(p5, "suvarilerin saldiri gucu:%.2lf\n", insanlar.suvariler.toplam_saldiri);
				if (insanlar.kusatma_makineleri.birim_adet > 0)
					fprintf(p5, "kusatma makinelerinin saldiri gucu:%.2lf\n", insanlar.kusatma_makineleri.toplam_saldiri);

				fputc('\n', p5);

				if (orklar.ork_dovusculeri.birim_adet > 0)
					fprintf(p5, "ork dovusculerinin savunma gucu:%.2lf\n", orklar.ork_dovusculeri.toplam_savunma);
				if (orklar.mizrakcilar.birim_adet > 0)
					fprintf(p5, "mizrakcilarin savunma gucu:%.2lf\n", orklar.mizrakcilar.toplam_savunma);
				if (orklar.varg_binicileri.birim_adet > 0)
					fprintf(p5, "varg binicilerinin savunma gucu:%.2lf\n", orklar.varg_binicileri.toplam_savunma);
				if (orklar.troller.birim_adet > 0)
					fprintf(p5, "trollerin savunma gucu:%.2lf\n", orklar.troller.toplam_savunma);

				fputc('\n', p5);

				fprintf(p5, "insanlarin toplam saldiri gucu:%.2lf\n", insan_saldiri);
				fprintf(p5, "orklarin toplam savunma gucu gucu:%.2lf\n", ork_savunma);
				fprintf(p5, "toplam net hasar:%.2lf\n\n", net_hasar);


				if (orklar.ork_dovusculeri.birim_adet > 0 && ork_saglik <= 0)
					fprintf(p5, "ork dovusculerinin saglik durumu:0\n");
				else if (orklar.ork_dovusculeri.birim_adet > 0 && orklar.ork_dovusculeri.toplam_saglik >= 0)
					fprintf(p5, "ork dovusculerinin saglik durumu:%.2lf\n", orklar.ork_dovusculeri.toplam_saglik);
				else if (orklar.ork_dovusculeri.birim_adet > 0 && orklar.ork_dovusculeri.toplam_saglik < 0)
					fprintf(p5, "ork dovusculerinin saglik durumu:0.00\n");

				if (orklar.mizrakcilar.birim_adet > 0 && ork_saglik <= 0)
					fprintf(p5, "mizrakcilarin saglik durumu:0\n");
				else if (orklar.mizrakcilar.birim_adet > 0 && orklar.mizrakcilar.toplam_saglik >= 0)
					fprintf(p5, "mizrakcilarin saglik durumu:%.2lf\n", orklar.mizrakcilar.toplam_saglik);
				else if (orklar.mizrakcilar.birim_adet > 0 && orklar.mizrakcilar.toplam_saglik < 0)
					fprintf(p5, "mizrakcilarin saglik durumu:0.00\n");

				if (orklar.varg_binicileri.birim_adet > 0 && ork_saglik <= 0)
					fprintf(p5, "varg binicilerinin saglik durumu:0\n");
				else if (orklar.varg_binicileri.birim_adet > 0 && orklar.varg_binicileri.toplam_saglik >= 0)
					fprintf(p5, "varg binicilerinin saglik durumu:%.2lf\n", orklar.varg_binicileri.toplam_saglik);
				else if (orklar.varg_binicileri.birim_adet > 0 && orklar.varg_binicileri.toplam_saglik < 0)
					fprintf(p5, "varg binicilerinin saglik durumu:0.00\n");

				if (orklar.troller.birim_adet > 0 && ork_saglik <= 0)
					fprintf(p5, "trollerin saglik durumu:0\n");
				else if (orklar.troller.birim_adet > 0 && orklar.troller.toplam_saglik >= 0)
					fprintf(p5, "trollerin saglik durumu:%.2lf\n\n", orklar.troller.toplam_saglik);
				else if (orklar.troller.birim_adet > 0 && orklar.troller.toplam_saglik < 0)
					fprintf(p5, "trollerin saglik durumu:0.00\n");

				fputc('\n', p5);

				if (ork_saglik >= 0)
					fprintf(p5, "orklarin toplam saglik degeri:%.2lf\n", ork_saglik);

				else if (ork_saglik < 0)
					fprintf(p5, "orklarin toplam saglik degeri:0\n");

			}



			if (ork_saldiri > insan_savunma && ork_saglik > 0) {

				fprintf(p5, "\n******************************\n");
				fprintf(p5, "ADIM %d:ork saldirisi\n", ++sayac);
				fprintf(p5, "******************************\n\n");

				net_hasar = ork_saldiri - insan_savunma;

				insanlar.piyadeler.toplam_saglik -= (insanlar.piyadeler.toplam_savunma / insan_savunma) * net_hasar;
				insanlar.piyadeler.toplam_yenilen += (insanlar.piyadeler.toplam_savunma / insan_savunma) * net_hasar;

				insanlar.okcular.toplam_saglik -= (insanlar.okcular.toplam_savunma / insan_savunma) * net_hasar;
				insanlar.okcular.toplam_yenilen += (insanlar.okcular.toplam_savunma / insan_savunma) * net_hasar;

				insanlar.suvariler.toplam_saglik -= (insanlar.suvariler.toplam_savunma / insan_savunma) * net_hasar;
				insanlar.suvariler.toplam_yenilen += (insanlar.suvariler.toplam_savunma / insan_savunma) * net_hasar;

				insanlar.kusatma_makineleri.toplam_saglik -= (insanlar.kusatma_makineleri.toplam_savunma / insan_savunma) * net_hasar;
				insanlar.kusatma_makineleri.toplam_yenilen += (insanlar.kusatma_makineleri.toplam_savunma / insan_savunma) * net_hasar;

				insan_saglik -= net_hasar;



				if (orklar.ork_dovusculeri.birim_adet > 0)
					fprintf(p5, "ork dovusculerinin saldiri gucu:%.2lf\n", orklar.ork_dovusculeri.toplam_saldiri);
				if (orklar.mizrakcilar.birim_adet > 0)
					fprintf(p5, "mizrakcilarin saldiri gucu:%.2lf\n", orklar.mizrakcilar.toplam_saldiri);
				if (orklar.varg_binicileri.birim_adet > 0)
					fprintf(p5, "varg binicilerinin saldiri gucu:%.2lf\n", orklar.varg_binicileri.toplam_saldiri);
				if (orklar.troller.birim_adet > 0)
					fprintf(p5, "troller saldiri gucu:%.2lf\n\n", orklar.troller.toplam_saldiri);

				fputc('\n', p5);

				if (insanlar.piyadeler.birim_adet > 0)
					fprintf(p5, "piyadelerin savunma gucu:%.2lf\n", insanlar.piyadeler.toplam_savunma);
				if (insanlar.okcular.birim_adet > 0)
					fprintf(p5, "okcularin savunma gucu:%.2lf\n", insanlar.okcular.toplam_savunma);
				if (insanlar.suvariler.birim_adet > 0)
					fprintf(p5, "suvarilerin savunma gucu:%.2lf\n", insanlar.suvariler.toplam_savunma);
				if (insanlar.kusatma_makineleri.birim_adet > 0)
					fprintf(p5, "kusatma makinelerinin savunma gucu:%.2lf\n\n", insanlar.kusatma_makineleri.toplam_savunma);

				fputc('\n', p5);

				fprintf(p5, "orklarin toplam saldiri gucu:%.2lf\n", ork_saldiri);
				fprintf(p5, "insanlarin toplam savunma gucu gucu:%.2lf\n", insan_savunma);
				fprintf(p5, "toplam net hasar:%.2lf\n\n", net_hasar);


				if (insanlar.piyadeler.birim_adet > 0 && insan_saglik <= 0)
					fprintf(p5, "piyadelerin saglik durumu:0\n");
				else if (insanlar.piyadeler.birim_adet > 0 && insanlar.piyadeler.toplam_saglik >= 0)
					fprintf(p5, "piyadelerin saglik durumu:%.2lf\n", insanlar.piyadeler.toplam_saglik);
				else if (insanlar.piyadeler.birim_adet > 0 && insanlar.piyadeler.toplam_saglik < 0)
					fprintf(p5, "piyadelerin saglik durumu:0.00\n");

				if (insanlar.okcular.birim_adet > 0 && insan_saglik <= 0)
					fprintf(p5, "okcularin saglik durumu:0\n");
				else if (insanlar.okcular.birim_adet > 0 && insanlar.okcular.toplam_saglik >= 0)
					fprintf(p5, "okcularin saglik durumu:%.2lf\n", insanlar.okcular.toplam_saglik);
				else if (insanlar.okcular.birim_adet > 0 && insanlar.okcular.toplam_saglik < 0)
					fprintf(p5, "okcularin saglik durumu:0.00\n");

				if (insanlar.suvariler.birim_adet > 0 && insan_saglik <= 0)
					fprintf(p5, "suvarilerin saglik durumu:0\n");
				else if (insanlar.suvariler.birim_adet > 0 && insanlar.suvariler.toplam_saglik >= 0)
					fprintf(p5, "suvarilerin saglik durumu:%.2lf\n", insanlar.suvariler.toplam_saglik);
				else if (insanlar.suvariler.birim_adet > 0 && insanlar.suvariler.toplam_saglik < 0)
					fprintf(p5, "suvarilerin saglik durumu:0.00\n");

				if (insanlar.kusatma_makineleri.birim_adet > 0 && insan_saglik <= 0)
					fprintf(p5, "kusatma makinelerinin saglik durumu:0\n");
				else if (insanlar.kusatma_makineleri.birim_adet > 0 && insanlar.kusatma_makineleri.toplam_saglik >= 0)
					fprintf(p5, "kusatma makinelerinin saglik durumu:%.2lf\n\n", insanlar.kusatma_makineleri.toplam_saglik);
				else if (insanlar.kusatma_makineleri.birim_adet > 0 && insanlar.kusatma_makineleri.toplam_saglik < 0)
					fprintf(p5, "kusatma makinelerinin saglik durumu:0.00\n");

				fputc('\n', p5);

				if (insan_saglik >= 0)
					fprintf(p5, "insanlarin toplam saglik degeri:%.2lf\n", insan_saglik);

				else
					fprintf(p5, "insanlarin toplam saglik degeri:0\n");
			}

			sayac++;
		}



		if (insan_saglik > 0) {
			printf("\ninsan imparatorlugu kazandi\n\n");

			sayacc = 1;

			while (sayacc) {
				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT)
						sayacc = 0;
				}



				SDL_SetRenderDrawColor(olusturucu, 128, 0, 128, 255);
				SDL_RenderClear(olusturucu);

				int kenar = ekran.h / 20;

				for (int i = 0;i < kenar; i++) {

					for (int j = 0;j < kenar;j++) {

						if (i * kenar >= 10 * kenar) {

							SDL_Rect izgara = { i * kenar,j * kenar,kenar,kenar };

							SDL_SetRenderDrawColor(olusturucu, 0, 0, 255, 255);
							SDL_RenderFillRect(olusturucu, &izgara);
						}
					}
				}

				for (int i = 0;i < kenar; i++) {

					for (int j = 0;j < kenar;j++) {

						SDL_SetRenderDrawColor(olusturucu, 0, 0, 0, 255);

						SDL_Rect izgara = { i * kenar,j * kenar,kenar,kenar };
						SDL_RenderDrawRect(olusturucu, &izgara);
					}
				}

				kontrol1 = 1, kontrol2 = 1, kontrol3 = 1, kontrol4 = 1, say = 1, yerlestirme = 0, biriktir = 0;

				for (int i = 0;i < 10;i++) {
					for (int j = 0;j < 20;j++) {

						if (insanlar.piyadeler.birim_adet > 0 && insanlar.piyadeler.toplam_saglik > 0 && kontrol1 == 1) {

							yerlestirme = ((int)insanlar.piyadeler.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((insanlar.piyadeler.toplam_saglik * 100) / (insanlar.piyadeler.saglik * insanlar.piyadeler.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk >= 0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 5 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);



							SDL_Surface* yuz = IMG_Load(piyade);
							piyade_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 12 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, piyade_texture, NULL, &hedef);

							if (say * 100 >= insanlar.piyadeler.birim_adet) {
								kontrol1 = 0;
								say = 0;
								biriktir = 0;
							}

							say++;
						}

						else if (insanlar.okcular.birim_adet > 0 && insanlar.okcular.toplam_saglik > 0 && kontrol2 == 1) {

							yerlestirme = ((int)insanlar.okcular.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((insanlar.okcular.toplam_saglik * 100) / (insanlar.okcular.saglik * insanlar.okcular.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk>0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 5 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);



							SDL_Surface* yuz = IMG_Load(okcu);
							okcu_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 12 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, okcu_texture, NULL, &hedef);

							if (say * 100 >= insanlar.okcular.birim_adet) {
								kontrol2 = 0;
								say = 0;
								biriktir = 0;
							}

							say++;
						}

						else if (insanlar.suvariler.birim_adet > 0 && insanlar.suvariler.toplam_saglik > 0 && kontrol3 == 1) {

							yerlestirme = ((int)insanlar.suvariler.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((insanlar.suvariler.toplam_saglik * 100) / (insanlar.suvariler.saglik * insanlar.suvariler.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk>0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 5 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);



							SDL_Surface* yuz = IMG_Load(suvari);
							suvari_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 12 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, suvari_texture, NULL, &hedef);

							if (say * 100 >= insanlar.suvariler.birim_adet) {
								kontrol3 = 0;
								say = 0;
								biriktir = 0;
							}

							say++;
						}

						else if (insanlar.kusatma_makineleri.birim_adet > 0 && insanlar.kusatma_makineleri.toplam_saglik > 0 && kontrol4 == 1) {

							yerlestirme = ((int)insanlar.kusatma_makineleri.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 15 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((insanlar.kusatma_makineleri.toplam_saglik * 100) / (insanlar.kusatma_makineleri.saglik * insanlar.kusatma_makineleri.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk>0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 5 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);



							SDL_Surface* yuz = IMG_Load(kusatma);
							kusatma_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 12 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, kusatma_texture, NULL, &hedef);

							if (say * 100 >= insanlar.kusatma_makineleri.birim_adet)
								kontrol4 = 0;

							say++;
						}
					}
				}

				SDL_RenderPresent(olusturucu);
			}

			SDL_DestroyTexture(piyade_texture);
			SDL_DestroyTexture(okcu_texture);
			SDL_DestroyTexture(suvari_texture);
			SDL_DestroyTexture(kusatma_texture);
		}
		else if (ork_saglik > 0) {
			printf("\nork legi kazandi\n\n");

			sayacc = 1;

			while (sayacc) {
				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT)
						sayacc = 0;
				}



				SDL_SetRenderDrawColor(olusturucu, 128, 0, 128, 255);
				SDL_RenderClear(olusturucu);

				int kenar = ekran.h / 20;

				for (int i = 0;i < kenar; i++) {

					for (int j = 0;j < kenar;j++) {

						if (i * kenar >= 10 * kenar) {

							SDL_Rect izgara = { i * kenar,j * kenar,kenar,kenar };

							SDL_SetRenderDrawColor(olusturucu, 0, 0, 255, 255);
							SDL_RenderFillRect(olusturucu, &izgara);
						}
					}
				}

				for (int i = 0;i < kenar; i++) {

					for (int j = 0;j < kenar;j++) {

						SDL_SetRenderDrawColor(olusturucu, 0, 0, 0, 255);

						SDL_Rect izgara = { i * kenar,j * kenar,kenar,kenar };
						SDL_RenderDrawRect(olusturucu, &izgara);
					}
				}

				kontrol1 = 1, kontrol2 = 1, kontrol3 = 1, kontrol4 = 1, say = 1, yerlestirme = 0, biriktir = 0;

				for (int i = 0;i < 10;i++) {
					for (int j = 0;j < 20;j++) {

						if (orklar.ork_dovusculeri.birim_adet > 0 && orklar.ork_dovusculeri.toplam_saglik > 0 && kontrol1 == 1) {

							yerlestirme = ((int)orklar.ork_dovusculeri.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((orklar.ork_dovusculeri.toplam_saglik * 100) / (orklar.ork_dovusculeri.saglik * orklar.ork_dovusculeri.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk>0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 405 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);


							SDL_Surface* yuz = IMG_Load(ork);
							ork_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 412 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, ork_texture, NULL, &hedef);

							if (say * 100 >= orklar.ork_dovusculeri.birim_adet) {
								kontrol1 = 0;
								say = 0;
								biriktir = 0;
							}

							say++;
						}

						else if (orklar.mizrakcilar.birim_adet > 0 && orklar.mizrakcilar.toplam_saglik > 0 && kontrol2 == 1) {

							yerlestirme = ((int)orklar.mizrakcilar.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((orklar.mizrakcilar.toplam_saglik * 100) / (orklar.mizrakcilar.saglik * orklar.mizrakcilar.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk>0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 405 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);


							SDL_Surface* yuz = IMG_Load(mizrakci);
							mizrakci_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 412 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, mizrakci_texture, NULL, &hedef);

							if (say * 100 >= orklar.mizrakcilar.birim_adet) {
								kontrol2 = 0;
								say = 0;
								biriktir = 0;
							}

							say++;
						}

						else if (orklar.varg_binicileri.birim_adet > 0 && orklar.varg_binicileri.toplam_saglik > 0 && kontrol3 == 1) {

							yerlestirme = ((int)orklar.varg_binicileri.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((orklar.varg_binicileri.toplam_saglik * 100) / (orklar.varg_binicileri.saglik * orklar.varg_binicileri.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk>0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 405 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);



							SDL_Surface* yuz = IMG_Load(varg);
							varg_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 412 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, varg_texture, NULL, &hedef);

							if (say * 100 >= orklar.varg_binicileri.birim_adet) {
								kontrol3 = 0;
								say = 0;
								biriktir = 0;
							}

							say++;
						}

						else if (orklar.troller.birim_adet > 0 && orklar.troller.toplam_saglik > 0 && kontrol4 == 1) {

							yerlestirme = ((int)orklar.troller.birim_adet - biriktir) % 100;
							if (yerlestirme == 0)
								yerlestirme = 100;
							biriktir += yerlestirme;

							char birim_adet_str[20];
							sprintf(birim_adet_str, "%d", yerlestirme);

							TTF_Font* font = TTF_OpenFont(roboto, 48);
							SDL_Color color = { 255, 255, 255, 255 };
							SDL_Surface* yuzey_yazi = TTF_RenderText_Blended(font, birim_adet_str, color);
							SDL_Texture* yazi = SDL_CreateTextureFromSurface(olusturucu, yuzey_yazi);
							SDL_Rect yazi_bilgisi = { 415 + 40 * i,-1 + 40 * j,20,20 };
							SDL_FreeSurface(yuzey_yazi);
							SDL_RenderCopy(olusturucu, yazi, NULL, &yazi_bilgisi);



							bar_doluluk = (int)(((orklar.troller.toplam_saglik * 100) / (orklar.troller.saglik * orklar.troller.birim_adet)) / 5);

							if (bar_doluluk >= 15 && bar_doluluk <= 20)
								SDL_SetRenderDrawColor(olusturucu, 0, 255, 0, 255);

							else if (bar_doluluk < 15 && bar_doluluk >= 8)
								SDL_SetRenderDrawColor(olusturucu, 255, 255, 0, 255);

							else if (bar_doluluk < 8 && bar_doluluk>0)
								SDL_SetRenderDrawColor(olusturucu, 255, 0, 0, 255);

							if (bar_doluluk < 2)
								bar_doluluk = 2;

							SDL_Rect bar = { 405 + 40 * i, round(bar_doluluk * (-1.5) + 42) + 40 * j, 10, bar_doluluk };
							SDL_RenderFillRect(olusturucu, &bar);



							SDL_Surface* yuz = IMG_Load(troll);
							troll_texture = SDL_CreateTextureFromSurface(olusturucu, yuz);
							SDL_FreeSurface(yuz);

							SDL_Rect hedef;
							hedef.x = 412 + 40 * i;
							hedef.y = 12 + 40 * j;
							hedef.w = 30;
							hedef.h = 30;

							SDL_RenderCopy(olusturucu, troll_texture, NULL, &hedef);

							if (say * 100 >= orklar.troller.birim_adet)
								kontrol4 = 0;

							say++;
						}
					}

					SDL_RenderPresent(olusturucu);
				}
			}

			SDL_DestroyTexture(ork_texture);
			SDL_DestroyTexture(mizrakci_texture);
			SDL_DestroyTexture(varg_texture);
			SDL_DestroyTexture(troll_texture);

		}
		else
			printf("\nbir sorun var\n\n");



		SDL_DestroyWindow(pencere);
		SDL_DestroyRenderer(olusturucu);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();

	}

	fclose(senaryo_p);
	fclose(p1);
	fclose(p2);
	fclose(p3);
	fclose(p4);
	fclose(p5);

	return 0;
}

