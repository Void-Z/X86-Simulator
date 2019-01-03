// #include <stdio.h>
// #include <assert.h>

// int main() {
//   FILE *fp = fopen("/share/texts/num", "r+");
//   assert(fp);

//   fseek(fp, 0, SEEK_END);
//   long size = ftell(fp);
//   assert(size == 5000);

//   fseek(fp, 500 * 5, SEEK_SET);
//   int i, n;
//   for (i = 500; i < 1000; i ++) {
//     fscanf(fp, "%d", &n);
//     assert(n == i + 1);
//   }

//   fseek(fp, 0, SEEK_SET);
//   for (i = 0; i < 500; i ++) {
//     fprintf(fp, "%4d\n", i + 1 + 1000);
//   }

//   for (i = 500; i < 1000; i ++) {
//     fscanf(fp, "%d", &n);
//     assert(n == i + 1);
//   }

//   fseek(fp, 0, SEEK_SET);
//   for (i = 0; i < 500; i ++) {
//     fscanf(fp, "%d", &n);
//     assert(n == i + 1 + 1000);
//   }

//   fclose(fp);

//   printf("PASS!!!\n");

//   return 0;
// }

#include <stdio.h>
#include <assert.h>

int main() {
  FILE *fp = fopen("/share/texts/num", "r+");
  assert(fp);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  assert(size == 5000);

  fseek(fp, 500 * 5, SEEK_SET);
  size = ftell(fp);
  printf("55:%d\n",size);
  int i, n=0;
  // fscanf(fp, "%c", &n);
  for (i = 500; i < 1000; i ++) {
    // n = i + 1;
    // fscanf(fp, "%d", &n);
    // size = ftell(fp);
    // printf("60:%d\n",size);
    printf("61:%d == %d\n",n,i + 1);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  size = ftell(fp);
  printf("66:%d\n",size);
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }
  size = ftell(fp);
  printf("71:%d\n",size);
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }

  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
