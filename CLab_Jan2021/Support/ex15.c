
struct Student {
	int age;
	int schoolarsh[3];
	struct Student (*pfunction) (struct Student, struct Student); // pointer to function
};

struct Student addStudsBySchoolarsh(struct Student x, struct Student y) {
	struct Student studAdd;
	int i = 0;
	studAdd.age = (x.age + y.age)/2;
	for (i = 0; i < 3; i++)
		studAdd.schoolarsh[i] = x.schoolarsh[i] + y.schoolarsh[i];
	return studAdd;
}

void main()
{
	struct Student a1, a2, a3;

	a1.age= 21;
	a1.schoolarsh[0] = 30;
	a1.schoolarsh[1] = 32;
	a1.schoolarsh[2] = 35;

	a2.age = 23;
	a2.schoolarsh[0] = 40;
	a2.schoolarsh[1] = 42;
	a2.schoolarsh[2] = 43;

	// a3 = addStudsBySchoolarsh(a1, a2);
	a1.pfunction = addStudsBySchoolarsh;
	a3 = a1.pfunction(a1, a2);

	a2 = a3.pfunction(a1, a3);
}
