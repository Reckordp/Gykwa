#include <unistd.h>

#define PAPAN (1)

typedef struct berita_gykwa bGykwa;
typedef __uint8_t uint8_t;

static inline void sampaikan_murni(const char *s, size_t u) {
	write(PAPAN, s, u);
}
void sampaikan(const char*);
void sampaikan_angka(unsigned long);
static inline void kosongkan_area(void *ptr, size_t ukuran) {
	uint8_t *penunjuk = (uint8_t*)ptr;
	while(ukuran--) *penunjuk++ = 0;
}

struct deretan_karcis;
struct deretan_karcis *nomorkan_alat_tersedia(const char*);
int aph_alat_didukung(struct deretan_karcis*, const char*);
void matikan_penomoran(struct deretan_karcis*);
