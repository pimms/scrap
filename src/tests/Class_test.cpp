#include "ScrapTest.h"


TEST (ClassTest, TestCommonGetters)
{
	const int id = 13;
	const string name = "Class13";
	Class c(id, name);
	Class super(0, "SuperDuper");

	ASSERT_EQ(id, c.GetClassID());
	ASSERT_EQ(name, c.GetClassName());
	ASSERT_EQ(NULL, c.GetSuper());

	c.SetSuper(&super);
	ASSERT_EQ(&super, c.GetSuper());
}


TEST (ClassTest, TestValidSuperAssignment)
{
	Class super(0, "Super");
	Class sub(1, "Sub");

	ASSERT_NO_THROW(sub.SetSuper(&super));
	ASSERT_EQ(&super, sub.GetSuper());
}

TEST (ClassTest, TestNullSuperAssignment)
{
	Class sub(1, "Sub");
	ASSERT_ANY_THROW(sub.SetSuper(NULL));
}

TEST (ClassTest, TestLockedSuperAssignment)
{
	{
		// Locking the sub 
		Class super(0, "Super");
		Class sub(1, "Sub");

		sub.LockMembers();
		ASSERT_ANY_THROW(sub.SetSuper(&super));
	}
	{
		// Locking the super
		Class super(0, "Super");
		Class sub(1, "Sub");

		super.LockMembers();
		ASSERT_ANY_THROW(sub.SetSuper(&super));
	}
}


TEST (ClassTest, TestValidMemberAssignment)
{
	Class c(0, "Class");
	
	// Method addition
	Method *norm = CreateMethod(&c, METHOD_NORMAL);
	Method *stat = CreateMethod(&c, METHOD_STATIC);

	ASSERT_NO_THROW(c.AddMethod(norm));
	ASSERT_NO_THROW(c.AddStaticMethod(stat));
	ASSERT_NO_THROW(c.AddFieldTemplate(TypeDesc{INT}));
	ASSERT_NO_THROW(c.AddStaticField(TypeDesc{INT}));

	delete norm;
	delete stat;
}

TEST (ClassTest, TestInvalidMemberAssignment)
{
	Class c(0, "Class");

	Method *norm = CreateMethod(&c, METHOD_NORMAL);
	Method *stat = CreateMethod(&c, METHOD_STATIC);

	ASSERT_ANY_THROW(c.AddMethod(stat));
	ASSERT_ANY_THROW(c.AddStaticMethod(norm));
	ASSERT_ANY_THROW(c.AddMethod(NULL));
	ASSERT_ANY_THROW(c.AddStaticMethod(NULL));

	delete norm;
	delete stat;
}

TEST (ClassTest, TestLockedMemberAssignment)
{
	// Much like "TestValidMemberAssignment", but the 
	// class-object is locked. 
	Class c(0, "Class");
	
	Method *norm = CreateMethod(&c, METHOD_NORMAL);
	Method *stat = CreateMethod(&c, METHOD_STATIC);

	c.LockMembers();

	ASSERT_ANY_THROW(c.AddMethod(norm));
	ASSERT_ANY_THROW(c.AddStaticMethod(stat));
	ASSERT_ANY_THROW(c.AddFieldTemplate(TypeDesc{INT}));
	ASSERT_ANY_THROW(c.AddStaticField(TypeDesc{INT}));

	delete norm;
	delete stat;
}

TEST (ClassTest, TestSuperInheritance)
{
	Class super(0, "Super");
	Class sub(1, "Sub");
	const Field* field;
	Method *method = CreateMethod(&super, METHOD_NORMAL);

	// Add an INT field and a method to super and a FLOAT field to sub
	super.AddFieldTemplate(TypeDesc{INT});
	super.AddMethod(method);
	sub.SetSuper(&super);
	sub.AddFieldTemplate(TypeDesc{FLOAT});

	ASSERT_NO_THROW(field = super.GetField(0));
	ASSERT_EQ(field->typeDesc.type, INT);

	ASSERT_NO_THROW(field = sub.GetField(0));
	ASSERT_EQ(field->typeDesc.type, INT);

	ASSERT_NO_THROW(field = sub.GetField(1));
	ASSERT_EQ(field->typeDesc.type, FLOAT);

	ASSERT_EQ(sub.GetMethod(0), super.GetMethod(0));
}
