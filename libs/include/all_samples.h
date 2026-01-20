#include "number_zero.h"
#include "number_one.h"
#include "number_two.h"
#include "number_three.h"
#include "number_four.h"
#include "number_five.h"
#include "number_six.h"
#include "number_seven.h"
#include "number_eight.h"
#include "number_nine.h"

// Array de ponteiros para facilitar o acesso via índice
const uint8_t* samples_db[10] = {
    mnist_sample_0_28x28, mnist_sample_1_28x28, mnist_sample_2_28x28,
    mnist_sample_3_28x28, mnist_sample_4_28x28, mnist_sample_5_28x28,
    mnist_sample_6_28x28, mnist_sample_7_28x28, mnist_sample_8_28x28,
    mnist_sample_9_28x28
};