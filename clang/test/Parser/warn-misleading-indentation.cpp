// RUN: %clang_cc1 -x c -fsyntax-only -verify %s
// RUN: %clang_cc1 -x c -fsyntax-only -verify -Wmisleading-indentation -DWITH_WARN %s
// RUN: %clang_cc1 -std=c++17 -fsyntax-only -verify -Wall -Wno-unused -DWITH_WARN -DCXX17 %s
// RUN: %clang_cc1 -std=c++17 -fsyntax-only -verify -Wall -Wno-unused -Wno-misleading-indentation -DCXX17 %s

#ifndef WITH_WARN
// expected-no-diagnostics
#endif

void f0(int i) {
  if (i)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
    i = i + 1;
    int x = 0;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'if'}}
#endif
  return;
#ifdef CXX17
  if constexpr (false)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
    i = 0;
    i += 1;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'if'}}
#endif
#endif
}

void f1(int i) {
  for (;i;)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
    i = i + 1;
    i *= 2;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'for'}}
#endif
  return;
}

void f2(int i) {
  while (i)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
    i = i + 1; i *= 2;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'while'}}
#endif
  return;
}

void f3(int i) {
  if (i)
    i = i + 1;
  else
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
    i *= 2;
    const int x = 0;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'else'}}
#endif
}

#ifdef CXX17
struct Range {
  int *begin() {return nullptr;}
  int *end() {return nullptr;}
};
#endif

void f4(int i) {
  if (i)
  i *= 2;
  return;
  if (i)
    i *= 2;
    ;
  if (i)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
    i *= 2;
    typedef int Int;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'if'}}
#endif
#ifdef CXX17
  Range R;
  for (auto e : R)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
    i *= 2;
    using Int2 = int;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'for'}}
#endif
#endif
}

int bar(void);

int foo(int* dst)
{   
    if (dst)
       return
    bar();
  if (dst)
    dst = dst + \
    bar();
  return 0;
}

void g(int i) {
  if (1)
    i = 2;
  else
         if (i == 3)
#ifdef WITH_WARN
// expected-note@-3 {{here}}
#endif
    i = 4;
    i = 5;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'if'}}
#endif
}

// Or this
#define TEST i = 5
void g0(int i) {
  if (1)
    i = 2;
  else
    i = 5;
    TEST;
}

void g1(int i) {
  if (1)
    i = 2;
  else if (i == 3)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
      i = 4;
      i = 5;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'if'}}
#endif
}

void g2(int i) {
  if (1)
    i = 2;
  else
    if (i == 3)
    {i = 4;}
    i = 5;
}

void g6(int i) {
        if (1)
                if (i == 3)
#ifdef WITH_WARN
// expected-note@-2 {{here}}
#endif
                        i = 4;
                        i = 5;
#ifdef WITH_WARN
// expected-warning@-2 {{misleading indentation; statement is not part of the previous 'if'}}
#endif
}

void g7(int i) {
  if (1)
    i = 4;
#ifdef TEST1
#endif
    i = 5;
}

void a1(int i) { if (1) i = 4; return; }

void a2(int i) {
  {
    if (1)
      i = 4;
      }
  return;
}

void a3(int i) {
  if (1)
    {
    i = 4;
    }
    return;
}