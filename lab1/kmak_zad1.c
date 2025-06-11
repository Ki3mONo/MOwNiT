#include <stdio.h>
#include <float.h>
#include <math.h>


// Funkcja Pomocnicza dla podpunktu: liczba bajtów używana do przechowywania zmiennej danego typu,
//   Funkcja pomocnicza wyznaczająca rozmiar typów zmiennoprzecinkowych
//   (float, double, long double) na podstawie przesunięcia w pamięci.
  
//   W praktyce używamy tablic typu float (lub double / long double) o rozmiarze 2.
//   Następnie obliczamy różnicę adresów elementów tablicy [1] i [0].
//   Podobnie dla double i long double.
  
//   Różnica adresów w bajtach (odjęta jako wskaźniki na char*) wskazuje,
//   ile bajtów zajmuje jeden element (float, double, long double).

void print_size_of_format() {

    float f[2];
    double d[2];
    long double ld[2];

    printf("Rozmiar float (doświadczalnie): %td bajtów\n", (char*)(&f[1]) - (char*)(&f[0]));
    printf("Rozmiar double (doświadczalnie): %td bajtów\n", (char*)(&d[1]) - (char*)(&d[0]));
    printf("Rozmiar long double (doświadczalnie): %td bajtów\n", (char*)(&ld[1]) - (char*)(&ld[0]));
}


// Funkcje Pomocnicze dla podpunktu: liczba bitów na mantysę,
  
//   Idea polega na tym, aby zaczynając od x=1.0:
//   - dzielić x przez 2 (x /= 2) tak długo, aż wyrażenie (1.0 + x) == 1.0
//     (w przypadku float, 1.0f + x == 1.0f itd) przestanie być większe od 1.0.
  
//   Gdy (1.0 + x) == 1.0, oznacza to, że x "zniknęło" w mantysie –
//   zmiana jest zbyt mała, aby wpłynąć na wartość sumy w reprezentacji
//   zmiennoprzecinkowej. W ten sposób zliczamy liczbę podziałów przez 2,
//   która odzwierciedla liczbę bitów dostępnych w mantysie (mniej więcej,
//   bo w standardzie IEEE 754 jest jeszcze bit "ukryty").

int count_mantissa_bits_float() {
    float x = 1.0f;
    int bits = 0;

    while ((1.0f + x) != 1.0f) {
        x /= 2.0f;
        bits++;
    }
    return bits;
}

int count_mantissa_bits_double() {
    double x = 1.0;
    int bits = 0;

    while ((1.0 + x) != 1.0) {
        x /= 2.0;
        bits++;
    }
    return bits;
}

int count_mantissa_bits_long_double() {
    long double x = 1.0L;
    int bits = 0;

    while ((1.0L + x) != 1.0L) {
        x /= 2.0L;
        bits++;
    }
    return bits;
}

void print_mantissa_bits() {
    printf("Liczba bitów mantysy (float): %d\n", count_mantissa_bits_float());
    printf("Liczba bitów mantysy (double): %d\n", count_mantissa_bits_double());
    printf("Liczba bitów mantysy (long double): %d\n", count_mantissa_bits_long_double());

}


// Funkcje Pomocnicze dla podpunktu: liczba bitów na cechę (wliczając znak),

// - Zaczynamy od x=1.0, i wielokrotnie mnożymy x *= 2.0, 
// dopóki x nie przejdzie w stan infinity (isinf(x) == true).
// - Zliczamy, ile razy udało się x pomnożyć, zanim wynik stał się nieskończony.
// - Następnie liczba bitów w cechy to, log2(liczba tych operacji) + 1.


int count_exponent_bits_float() {
    float x = 1.0f;
    int exp = 0;
    int max_exp = 0;

    while (!isinf(x)) {
        x *= 2.0f;
        exp++;
        if (exp > max_exp) max_exp = exp;
    }

    return (int)(log2(max_exp) + 1);
}

int count_exponent_bits_double() {
    double x = 1.0;
    int exp = 0;
    int max_exp = 0;

    while (!isinf(x)) {
        x *= 2.0;
        exp++;
        if (exp > max_exp) max_exp = exp;
    }

    return (int)(log2(max_exp) + 1);
}

int count_exponent_bits_long_double() {
    long double x = 1.0L;
    int exp = 0;
    int max_exp = 0;

    while (!isinf(x)) {
        x *= 2.0L;
        exp++;
        if (exp > max_exp) max_exp = exp;
    }

    return (int)(log2(max_exp) + 1);
}

void print_exponent_bits() {
    printf("Liczba bitów cechy (float): %d\n", count_exponent_bits_float());
    printf("Liczba bitów cechy (double): %d\n", count_exponent_bits_double());
    printf("Liczba bitów cechy (long double): %d\n", count_exponent_bits_long_double());
}

// Funkcje Pomocnicze dla podpunktu: "maszynowe epsilon" - najmniejsza liczba, dla której 1.0+e > 1.0,
// - Startujemy z eps = 1.0 (dla float: 1.0f, dla double: 1.0, dla long double: 1.0L).
// - W pętli dzielimy eps przez 2, dopóki 1.0 + (eps/2) nadal jest większe od 1.0.
//   Jeśli 1.0 + (eps/2) == 1.0, oznacza to, że ta część (eps/2) "zniknęła" i nie 
//   jest już reprezentowana w sumie z 1.0 w danym formacie zmiennoprzecinkowym.

float compute_float_epsilon() {
    float eps = 1.0f;
    while (1.0f + eps / 2.0f > 1.0f) {
        eps /= 2.0f;
    }
    return eps;
}

double compute_double_epsilon() {
    double eps = 1.0;
    while (1.0 + eps / 2.0 > 1.0) {
        eps /= 2.0;
    }
    return eps;
}

long double compute_long_double_epsilon(void) {
    long double eps = 1.0L;
    while (1.0L + eps / 2.0L > 1.0L) {
        eps /= 2.0L;
    }
    return eps;
}

void print_eps(){
    printf("Maszynowe epsilon dla float: %g\n", compute_float_epsilon());
    printf("Maszynowe epsilon dla double: %g\n", compute_double_epsilon());
    long double ld_eps = compute_long_double_epsilon();
    printf("Maszynowe epsilon dla long double: %Lg\n", ld_eps);
}

// Funkcje Pomocnicze dla podpunktu: występowanie i sposób reprezentacji wartości specjalnych (±0, ±Inf, NaN).

// - Tworzymy +0 i -0 (odpowiednio 0.0f i -0.0f ird).
// - Tworzymy +Inf i -Inf, dzieląc przez zero.
// - Tworzymy NaN, biorąc np. sqrt(-1.0).
// - isnan(...) pozwala sprawdzić, czy dana wartość to NaN (1 = tak, 0 = nie).

void check_special_values_float() {
    float zero_p = 0.0f;
    float zero_n = -0.0f;
    float inf_p = 1.0f / zero_p;
    float inf_n = -1.0f / zero_p;
    float nan_v = sqrtf(-1.0f); // NaN

    printf("FLOAT:\n");
    printf("+0: %.1f | -0: %.1f\n", zero_p, zero_n);
    printf("+Inf: %f | -Inf: %f\n", inf_p, inf_n);
    printf("NaN: %f (czy NaN? %d)\n", nan_v, isnan(nan_v));
}

void check_special_values_double() {
    double zero_p = 0.0;
    double zero_n = -0.0;
    double inf_p = 1.0 / zero_p;
    double inf_n = -1.0 / zero_p;
    double nan_v = sqrt(-1.0); // NaN

    printf("\nDOUBLE:\n");
    printf("+0: %.1f | -0: %.1f\n", zero_p, zero_n);
    printf("+Inf: %f | -Inf: %f\n", inf_p, inf_n);
    printf("NaN: %f (czy NaN? %d)\n", nan_v, isnan(nan_v));
}

void check_special_values_long_double() {
    long double zero_p = 0.0L;
    long double zero_n = -0.0L;
    long double inf_p = 1.0L / zero_p;
    long double inf_n = -1.0L / zero_p;
    long double nan_v = sqrtl(-1.0L); // NaN

    printf("\nLONG DOUBLE:\n");
    printf("+0: %.1Lf | -0: %.1Lf\n", zero_p, zero_n);
    printf("+Inf: %Lf | -Inf: %Lf\n", inf_p, inf_n);
    printf("NaN: %Lf (czy NaN? %d)\n", nan_v, isnan(nan_v));
}

void print_special_values(){
    check_special_values_float();
    check_special_values_double();
    check_special_values_long_double();
}


// ZADANIE 1: 
// Wyznaczyć doświadczalnie parametry reprezentacji liczb zmiennoprzecinkowych
// (float, double, long double) i porównać uzyskane wartości dla różnych architektur,
// systemów operacyjnych i kompilatorów. Sprawdzić, czy reprezentacje są zgodne ze
// standardem IEEE.

int main(void) {

    // liczba bajtów używana do przechowywania zmiennej danego typu,
    printf("Rozmiar z użyciem sizeof:\n");
    printf("float: %llu\n", sizeof(float));
    printf("double: %llu\n", sizeof(double));
    printf("long double: %llu\n", sizeof(long double));

    printf("\nRozmiar wyznaczony doświadczalnie:\n");
    print_size_of_format();
    printf("----------------------------------\n");

    // liczba bitów na mantysę,
    printf("\nRozmiar mantysy wyznaczony doświadczalnie:\n");
    print_mantissa_bits();
    printf("----------------------------------\n");

    // liczba bitów na cechę (wliczając znak),
    printf("\nRozmiar cechy wyznaczony doświadczalnie:\n");
    print_exponent_bits();
    printf("----------------------------------\n");

    // "maszynowe epsilon" - najmniejsza liczba, dla której 1.0+e > 1.0,
    printf("\nMaszynowe epsilon wyznaczone doświadczalnie:\n");
    print_eps();
    printf("----------------------------------\n");

    // występowanie i sposób reprezentacji wartości specjalnych (±0, ±Inf, NaN).
    print_special_values();
    
    return 0;
}
