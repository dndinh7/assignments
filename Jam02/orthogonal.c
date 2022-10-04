#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int equals_zero(float dot_product) {
  double epsilon= 0.00001;

  // perp
  if (dot_product > -epsilon && dot_product < epsilon) return 1;
  
  // else return false
  return 0;
}

int main() {
  float v1;
  float v2;
  float v3;

  float u1;
  float u2;
  float u3;

  printf("Please enter the values for the first vector: ");
  scanf("%f %f %f", &v1, &v2, &v3);

  printf("Please enter the values for the second vector: ");
  scanf("%f %f %f", &u1, &u2, &u3);

  float dot_product= v1 * u1 + v2 * u2 + v3 * u3;

  printf("The dot product is %.2f\n", dot_product);
  if (equals_zero(dot_product)) printf("The vectors are PERPENDICULAR!\n");
  else printf("The vectors are not PERPENDICULAR!\n");

  return 0;
}
