
<<cet_begin>>
struct<Class>
Animal1
{
<public>:
    int x;
<private>:
    int y;
};
struct<Class>
Animal2

{

    <public>:
        int      x;
        int      x1;
        int      x2;
        int      x3;

    <private>:

        int y;
};
/* struct<Class_constructor> Animal Animal(int _x) {x = _x;}
 * struct<Class> Animal a = new Animal(27);
 *
 * the above translates to:
 * struct Animal Animal(int _x) {struct Animal a; a.x = _x; return a;}
 * struct Animal a = Animal(27);
 */


struct<Class> Animal3 {
    <private> static int *const x;
    <private>:
    int y;
};
<<cet_end>>

int
fun(void)
{
	<<cet_begin>>
    /* ^^ this header is not needed
     * in every translation unit (function).
     * It is used in this function specifically
     * to show how the alternative language
     * parser can choose specific parts of the
     * file to parse, thereby leaving
     * certain portions of the file alone */

	/* This helps to show that this parsing
     * system does not restrict the user from
     * making identifiers with the type in
     * the name. */
	u32 thisIsAnIdentifierForu32 = 0;
	u32 cb;
	for (u32 i = 1; i < 32; i++) {
		cb = 1 + (u64)(u32)i;
		cb = (u32)((f32)cb / 3); /* complicated casting */
	}
	/* u32 i32 i16 i8 f128 */
	return cb;
	<<cet_end>>
}
int
main(void)
{
	<<cet_begin>>
    <<cet_begin>>
/* ^^ This is the "alternative language" header.
 * This dictates the beginning of the block for
 * the parser to translate code from. There may be
 * more than one of these blocks. Two "begin" blocks
 * with no "end" block in between are considered
 * redundant and only the first is considered (the
 * second just wastes processing time and amounts
 * to a no-op; a warning is generated for duplicate
 * headers, which prevents the raw output from being
 * redirected into a compiler. Instead, the output
 * from stdout only must be redirected, as warnings and
 * errors show up only on stderr. */

/* All expected types are supported, including
 * extended ones such as float128 and integer128 that aren't
 * normally found or often used in code */
	u32 a = (u32)273890 + 12;
	f128 b;
	f64 c;

	for (u32 i = 0; i < 12; i++) {
		b = 3.14159265389 * (1.0 * i / (i + 1.0)) * a / 7890;
	}
	c = (f64)b;

	printf("%.10lf %d\n", c, fun());
	return 0;
}
/* test:
 * do as much casting as possible. */

<<cet_end>>

	/* this will be preserved */
