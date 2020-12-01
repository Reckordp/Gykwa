#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include "Gykwa.h"

extern inline void sampaikan_murni(const char*, size_t);

void sampaikan(const char *pesan) {
	size_t ukuran = 0;
	const char *t;
	char tahan[256];

	t = pesan;
	while(*t && ukuran < 256) tahan[ukuran++] = *t++;
	if (pesan[ukuran]) {
		sampaikan_murni((const char*)tahan, ukuran);
		sampaikan(pesan + ukuran);
	} else {
		tahan[ukuran++] = 10;
		sampaikan_murni((const char*)tahan, ukuran);
	}
}

void sampaikan_angka(unsigned long angka) {
	size_t ukuran = 0;
	char tahan[256];
	uint8_t pegang;

#define MASUKKAN(a) (tahan[256 - ++ukuran] = (a))
	MASUKKAN(10);

	if (!angka) MASUKKAN(48);
	else {
		while(angka) {
			MASUKKAN(angka % 10 + 48);
			angka /= 10;
		}
	}
#undef MASUKKAN

	sampaikan_murni((const char*)tahan + 256 - ukuran, ukuran);
}

struct karcis;

struct karcis {
	uint8_t nomor;
	struct karcis *depan;
};

struct deretan_karcis {
	char asal[1024];
	char daftar[256];
	struct karcis *rantai[256];
};

struct deretan_karcis *nomorkan_alat_tersedia(const char *larik_alat) {
	const char *t;
	char c, *d;
	int lewati; 
	struct deretan_karcis *dk;
	struct karcis *tahan[256], *k;
	size_t ukuran, pos, harga;

	dk = malloc(sizeof(struct deretan_karcis));
	kosongkan_area(dk, sizeof(struct deretan_karcis));
	ukuran = 0;
	t = larik_alat;
	lewati = 0;

	while((c = *t++)) {
		if (!lewati) {
			harga++;
			d = dk->daftar;
			while(*d && *d != c) d++;
			*d = c;

			k = malloc(sizeof(struct karcis));
			k->nomor = ukuran;
			k->depan = NULL;

			pos = d - dk->daftar;
			if (!dk->rantai[pos]) {
				dk->rantai[pos] = k;
				tahan[pos] = k;
			} else {
				tahan[pos]->depan = k;
				tahan[pos] = k;
			}
		}

		dk->asal[ukuran++] = c;
		lewati = (c != 32);
	}

	return dk;
}

int aph_alat_didukung(struct deretan_karcis *dk, const char *alat) {
	const char *t, *representasi;
	char c;
	struct karcis *k;

	t = alat;
	c = *t;
	representasi = dk->daftar;

	while (*representasi && *representasi != c) representasi++;
	k = dk->rantai[representasi - dk->daftar];

	while (k) {
		representasi = dk->asal + k->nomor;
		while(*t++ == *representasi++) {
			if (!(*t) && ((*representasi == 32) || (*representasi == 0))) return 1;
			if ((*representasi == 32) || !(*representasi)) break;
		}
		k = k->depan;
	}

	return 0;
}

void matikan_penomoran(struct deretan_karcis *dk) {
	struct karcis *k1, *k2;
	uint8_t nomor = 0;

	while((k1 = dk->rantai[nomor++])) {
		while((k2 = k1)) {
			k1 = k2->depan;
			free(k2);
		}
	}
	free(dk);
}