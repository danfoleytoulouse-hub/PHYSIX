#include <eadk.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define W 320
#define H 240
#define WHITE eadk_color_white
#define BLACK eadk_color_black
#define GRAY eadk_color_gray
#define BLUE eadk_color_blue

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "PHYSIX";
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

static const char *titles[] = {
  "Pile electrochimique", "Combustion et CO2", "Unites / methodes",
  "Exemples corriges", "Quiz"
};
static int selected = 0;
static int page = 0;
static int category = -1;

static void clear(void) {
  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, W, H}, WHITE);
}
static void text(const char *s, int x, int y, bool invert) {
  eadk_display_draw_string(s, (eadk_point_t){x, y}, invert, BLACK, WHITE);
}
static void line(int y) {
  eadk_display_push_rect_uniform((eadk_rect_t){12, y, 296, 1}, GRAY);
}
static void draw_home(void) {
  clear();
  text("PHYSIX", 14, 12, true);
  text("Revision Physique-Chimie", 14, 34, false);
  line(54);
  for (int i = 0; i < 5; i++) {
    if (i == selected) eadk_display_push_rect_uniform((eadk_rect_t){12, 62 + i * 27, 296, 22}, BLUE);
    eadk_display_draw_string(titles[i], (eadk_point_t){22, 64 + i * 27}, i == selected, i == selected ? WHITE : BLACK, i == selected ? BLUE : WHITE);
  }
  line(204);
  text("OK: ouvrir   Haut/Bas: choisir", 12, 214, false);
}

static const char *pile_pages[][8] = {
 {"PILE : PRINCIPES", "Oxydation = perte d'e-", "Reduction = gain d'e-", "Anode : oxydation", "Cathode : reduction", "Pile en fonctionnement :", "anode (-), cathode (+)", "Electrons anode -> cathode"},
 {"ZINC / ARGENT", "Zn -> Zn2+ + 2e-", "Oxydation au zinc", "Anode negative (-)", "Ag+ + e- -> Ag", "Reduction a l'argent", "Cathode positive (+)", "Zn + 2Ag+ -> Zn2+ + 2Ag"},
 {"FORMULES : PILE", "n = m / M", "n = C x V (V en L)", "Q = I x t", "Q = n(e-) x F", "F = 96500 C/mol", "I = U / R", "t(h) = t(s) / 3600"},
 {"REACTIF LIMITANT", "Equation : Zn + 2Ag+", "-> Zn2+ + 2Ag", "Comparer n0 / coefficient", "Zn : n0(Zn) / 1", "Ag+ : n0(Ag+) / 2", "Plus petit rapport :", "reactif limitant"},
 {"AUTONOMIE", "I = U / R", "Q = n(e-) x F", "t = Q / I", "Q en coulomb", "I en ampere", "t en seconde", "Convertir t en heures"}
};
static const char *comb_pages[][8] = {
 {"COMBUSTION COMPLETE", "CxHy + (x+y/4) O2", "-> x CO2 + (y/2) H2O", "Equilibrer C, puis H,", "puis O en dernier.", "Hexadecane : C16H34", "C16H34 + 24.5 O2", "-> 16 CO2 + 17 H2O"},
 {"FORMULES GAZOLE / CO2", "rho = m / V", "m = rho x V", "n = m / M", "M(CxHy)=12x+y", "M(CO2)=44 g/mol", "n(CO2)=16 n(C16H34)", "m(CO2)=44 n(CO2)"},
 {"EXEMPLE HEXADECANE", "M(C16H34)=226 g/mol", "3000 L = 3 m3", "m=770 x 3=2310 kg", "n=2310000/226", "n approx 10221 mol", "n(CO2)=16 x 10221", "m(CO2) approx 7.20 t"}
};
static const char *unit_pages[][8] = {
 {"UNITES", "mL -> L : / 1000", "50 mL = 0.050 L", "kg -> g : x 1000", "h -> s : x 3600", "s -> h : / 3600", "1 kg/m3 = 1 g/L", "Toujours indiquer unite"},
 {"METHODE EXAMEN", "1. Relever les donnees", "2. Convertir les unites", "3. Ecrire la formule", "4. Isoler l'inconnue", "5. Remplacer / calculer", "6. Unite + phrase", "Verifier la coherence"}
};
static const char *example_pages[][8] = {
 {"EXEMPLE PILE", "m(Zn)=5.0 g ; M=65.4", "n(Zn)=5/65.4=0.0765 mol", "n(Ag+)=0.10 x 0.050", "=0.0050 mol", "Ag+ limitant : 0.0050/2", "n(e-)=0.0050 mol", "Q=0.0050 x 96500=482.5 C"},
 {"EXEMPLE CO2", "V=3000 L = 3 m3", "m(gazole)=770 x 3", "=2310 kg", "M(C16H34)=226 g/mol", "n=2310000/226=10221", "n(CO2)=16 x 10221", "m(CO2) approx 7.20 tonnes"}
};
static const char *quiz_pages[][8] = {
 {"QUIZ - REPONSES", "Oxydation : perte d'e-", "Anode d'une pile : (-)", "Electrons : anode -> cathode", "M(CO2) = 44 g/mol", "50 mL = 0.050 L", "1 mol C16H34 -> 16 mol CO2", "Q = I.t ou n(e-).F"}
};

static void draw_page(void) {
  clear();
  const char *(*data)[8] = pile_pages;
  int count = 5;
  if (category == 1) { data = comb_pages; count = 3; }
  if (category == 2) { data = unit_pages; count = 2; }
  if (category == 3) { data = example_pages; count = 2; }
  if (category == 4) { data = quiz_pages; count = 1; }
  if (page >= count) page = 0;
  text(titles[category], 12, 10, true);
  line(32);
  for (int i = 0; i < 8; i++) text(data[page][i], 12, 43 + i * 20, false);
  line(208);
  text("OK/Suivant: page  Retour: menu", 12, 216, false);
}

int main(void) {
  while (1) {
    if (category < 0) draw_home(); else draw_page();
    eadk_keyboard_state_t k = eadk_keyboard_scan();
    if (category < 0) {
      if (eadk_keyboard_key_down(k, EADK_KEY_UP)) selected = (selected + 4) % 5;
      if (eadk_keyboard_key_down(k, EADK_KEY_DOWN)) selected = (selected + 1) % 5;
      if (eadk_keyboard_key_down(k, EADK_KEY_OK)) { category = selected; page = 0; }
    } else {
      if (eadk_keyboard_key_down(k, EADK_KEY_OK) || eadk_keyboard_key_down(k, EADK_KEY_RIGHT)) page++;
      if (eadk_keyboard_key_down(k, EADK_KEY_BACK) || eadk_keyboard_key_down(k, EADK_KEY_LEFT)) category = -1;
    }
    eadk_timing_msleep(120);
  }
  return 0;
}
