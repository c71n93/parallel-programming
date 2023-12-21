#include "common.hpp"

void show_algo()
{
    auto A_1 = initA_1();
    auto A = initA();
    auto Y = initY();
    double mer = 10.0;
    int curIter = 1;

    while (mer > 0.001)
    {
        auto F = Seq::calcF(Y);
        auto dif = Seq::Minus(F, Seq::Mult(A, Y));
        Y = Seq::Plus(Y, Seq::Div(Seq::Mult(A_1, dif), 10));

        mer = dumpMer(dif, std::to_string(curIter));

        curIter++;
    }

    dump(Y, "solution");
}

void measure_seq()
{
    auto A_1 = initA_1();
    auto A = initA();
    auto Y = initY();
    double mer = 10.0;
    int curIter = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (mer > 0.005)
    {
        auto F = Seq::calcF(Y);
        auto dif = Seq::Minus(F, Seq::Mult(A, Y));
        Y = Seq::Plus(Y, Seq::Div(Seq::Mult(A_1, dif), 10));
        mer = Seq::dumpMer(dif);
        ++curIter;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Sequential time : " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "us" << std::endl;
    std::cout << "Used " << curIter << " iterations\n";

    Seq::dumpToFile(Y, "solution_seq.txt");
}

void measure_par()
{
    auto A_1 = initA_1();
    auto A = initA();
    auto Y = initY();
    double mer = 10.0;
    int curIter = 0;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (mer > 0.005)
    {
        auto F = Par::calcF(Y);
        auto dif = Par::Minus(F, Par::Mult(A, Y));
        Y = Par::Plus(Y, Par::Div(Par::Mult(A_1, dif), 10));
        mer = Par::dumpMer(dif);
        ++curIter;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Parallel time : " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "us" << std::endl;
    std::cout << "Used " << curIter << " iterations\n";
    Par::dumpToFile(Y, "solution_par.txt");
}

long getNumber(char *numString);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./a.out <regime>");
        fprintf(stderr, "Regimes:\n"
                        "\t 0 -- show how algo works\n"
                        "\t 1 -- measure sequential\n"
                        "\t 2 -- measure parallel\n");
        exit(EXIT_FAILURE);
    }

    int regime = getNumber(argv[1]);
    if (!(regime == 0 || regime == 1 || regime == 2)) {
        fprintf(stderr, "Error: <regime> should be one from list (0,1,2) (%d was specified)\n", regime);
        exit(EXIT_FAILURE);
    }
    
    if (regime == 0) {
        // To show how algorithm works
        show_algo();
    } else if (regime == 1) {
        // To measure sequential work
        measure_seq();
    } else {
        // To measure parallel work
        measure_par();
    }
    
    exit(EXIT_SUCCESS);
}

long getNumber(char *numString)
{
    if (*numString == '\0')
    {
        fprintf(stderr, "empty number argument\n");
        return -1;
    }

    errno = 0;

    long gNumber;
    char* endOfEnter;

    const int baseOfNumber = 10;
    gNumber = strtol(numString, &endOfEnter, baseOfNumber);

    if(*endOfEnter != '\0')
    {
        fprintf(stderr, "strtol error\n");
        return -1;
    }
    if (errno != 0)
    {
        fprintf(stderr, "strtol error\n");
        return -1;
    }
    
    return gNumber;

}