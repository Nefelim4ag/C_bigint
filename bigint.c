#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bigint_print(uint8_t *num, size_t byte_len)
{
        uint8_t *ptr =  num + byte_len - 1;
        size_t i;
        printf("0x");
        for (i = 0; i < byte_len; i++) {
                printf("%2" PRIx32, *ptr);
                ptr--;
        }
        printf("\n");
}

void bigint_Lshift(uint8_t *num, size_t byte_len, size_t shift)
{
	size_t bit_len = byte_len * 8;
        size_t i;

	if (shift >= bit_len) {
		memset(num, 0, byte_len);
		return;
	}

	while (shift >= 8) {
                for (i = byte_len - 1; i > 0; i--) {
                        num[i] = num[i - 1];
                }
                num[i] = 0;
		shift -= 8;
	}

        if (shift == 0)
                return;

        for (i = byte_len - 1; i > 0; i--) {
		num[i] = (num[i] << shift) | (num[i - 1] >> (8 - shift));
	}

	num[i] <<= shift;
}

void test_bigint_Lshift(){
        uint64_t num[3] = {0, 0, 0};
        size_t shift = 66;
        num[1] = 1;
        printf("---\n");
        printf("Test bigint_Lshift << %lu\n", shift);
        bigint_print((uint8_t *) &num, sizeof(num));
        bigint_Lshift((uint8_t *) &num, sizeof(num), shift);
        bigint_print((uint8_t *) &num, sizeof(num));
        printf("---\n");
        shift = 1;
        num[0] = 1;
        printf("---\n");
        printf("Test bigint_Lshift << %lu\n", shift);
        bigint_print((uint8_t *) &num, sizeof(num[0]));
        bigint_Lshift((uint8_t *) &num, sizeof(num[0]), shift);
        bigint_print((uint8_t *) &num, sizeof(num[0]));
        printf("---\n");
}

void bigint_Rshift(uint8_t *num, size_t byte_len, size_t shift)
{
	size_t bit_len = byte_len * 8;
        size_t i;

	if (shift >= bit_len) {
		memset(num, 0, byte_len);
		return;
	}

	while (shift >= 8) {
                for (i = 0; i < byte_len - 1; i++) {
                        num[i] = num[i + 1];
                }
                num[i] = 0;
		shift -= 8;
	}

        if (shift == 0)
                return;

        for (i = 0; i < byte_len; i++) {
		num[i] = (num[i] >> shift) | (num[i + 1] << (8 - shift));
	}

	num[i] >>= shift;
}

void test_bigint_Rshift(){
        uint64_t num[3] = {0, 0, 0};
        size_t shift = 66;
        num[2] = 1;
        printf("---\n");
        printf("Test bigint_Rshift >> %lu\n", shift);
        bigint_print((uint8_t *) &num, sizeof(num));
        bigint_Rshift((uint8_t *) &num, sizeof(num), shift);
        bigint_print((uint8_t *) &num, sizeof(num));
        printf("---\n");
}

void bigint_sum(uint8_t *res, uint8_t *a, uint8_t *b, size_t byte_len) {
        size_t i;
        uint16_t sum;
        uint16_t a_16, b_16, r_16;
        uint8_t *ptr = (uint8_t *) &sum;

        memset(res, 0, byte_len);

        for (i = 0; i < byte_len - 1; i++) {
                a_16 = a[i];
                b_16 = b[i];
                r_16 = res[i];
                sum = a_16 + b_16 + r_16;

                if (sum < 256) {
                        res[i] = sum;
                        continue;
                } else {
                        res[i]      = ptr[0];
                        res[i + 1] += ptr[1];
                }
        }

        a_16 = a[i];
        b_16 = b[i];
        r_16 = res[i];
        sum = a_16 + b_16 + r_16;

        /* Avoid overflow */
        res[i] = ptr[0];
}

int bigint_cmp(uint8_t *a, uint8_t *b, size_t byte_len) {
        int i, diff;

        for (i = byte_len - 1; i >= 0; i--) {
                diff = a[i] - b[i];
                if (diff)
                        break;
        }

        return diff;
}

size_t bigint_ilog2(uint8_t *num, size_t byte_len) {
        uint8_t tmp[byte_len];
        size_t i = 0;

        memset(tmp, 0, byte_len);
        tmp[0] = 1;

        while (bigint_cmp(num, tmp, byte_len) > 0) {
                i++;
                bigint_Lshift((uint8_t *) &tmp, byte_len, 1);
        }

        return i;
}

static int ilog2(uint64_t v)
{
	int l = 0;
	while ((1UL << l) < v)
		l++;
	return l;
}

void test_bigint_ilog2() {
        uint32_t val = 131073;
        printf("ilog2(%u) = ?\n", val);
        printf("BigInt ilog2: %lu\n", bigint_ilog2((uint8_t *) &val, sizeof(val)));
        printf("ilog2: %i\n", ilog2(val));
}

#define SHIFT 12

int main()
{
        test_bigint_Lshift();
        test_bigint_Rshift();
        test_bigint_ilog2();
}
