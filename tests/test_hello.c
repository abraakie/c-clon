//
// Created by Aki Abramowski on 23.05.25.
//

#include "unity.h"
#include "library.h"

// Diese Funktion wird vor jedem Test ausgeführt
void setUp(void) {
    // Hier können Sie Initialisierungen vornehmen
}

// Diese Funktion wird nach jedem Test ausgeführt
void tearDown(void) {
    // Hier können Sie Aufräumarbeiten durchführen
}

// Ein einfacher Test für die hello-Funktion
void test_hello_function(void) {
    // Da hello() nur nach stdout schreibt, ist es schwierig direkt zu testen
    // In der Praxis würden wir die Funktion so umschreiben, dass sie testbar ist
    // Hier demonstrieren wir erstmal nur, dass sie ohne Absturz läuft
    hello();
    TEST_PASS(); // Test bestanden, wenn die Funktion nicht abstürzt
}

// Beispiel für einen weiteren Test mit einer neuen Funktion
// (Sie müssten diese Funktion noch in library.h/.c implementieren)
void test_add_numbers(void) {
    int result = add_numbers(2, 3);
    TEST_ASSERT_EQUAL_INT(5, result);

    result = add_numbers(-1, 1);
    TEST_ASSERT_EQUAL_INT(0, result);

    result = add_numbers(0, 0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// Hauptfunktion, die alle Tests ausführt
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_hello_function);
    RUN_TEST(test_add_numbers);

    return UNITY_END();
}