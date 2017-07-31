import lib


def bind_test(gen):
	gen.start('my_test')

	lib.bind_all_defaults(gen)

	# inject test code in the wrapper
	gen.insert_code('''\
struct base_class {
	int base_method() { return 4; }
	int base_method_override() { return 4; }
	virtual int virtual_method() { return 6; }
	int u{6};
	static int v;
	int override{4};
	static int static_override;
};

int base_class::v{7};
int base_class::static_override{1};

struct derived_class : base_class {
	int derived_method() { return 8; }
	int base_method_override() { return 8; }
	int virtual_method() override { return 9; }
	int override{12};
	static int static_override;
};

int derived_class::static_override{42};

int read_virtual_method_through_base_class(base_class &o) {
	return o.virtual_method();
}
''', True, False)

	base_conv =	gen.begin_class('base_class')
	gen.bind_constructor(base_conv, [])
	gen.bind_method(base_conv, 'base_method', 'int', [])
	gen.bind_method(base_conv, 'base_method_override', 'int', [])
	gen.bind_method(base_conv, 'virtual_method', 'int', [])
	gen.bind_members(base_conv, ['int u', 'int override'])
	gen.bind_static_members(base_conv, ['int v', 'int static_override'])
	gen.end_class(base_conv)

	derived_conv = gen.begin_class('derived_class')
	gen.add_base(derived_conv, base_conv)
	gen.bind_constructor(derived_conv, [])
	gen.bind_method(derived_conv, 'derived_method', 'int', [])
	gen.bind_method(derived_conv, 'base_method_override', 'int', [])
	gen.bind_members(derived_conv, ['int override'])
	gen.bind_static_members(derived_conv, ['int static_override'])
	gen.end_class(derived_conv)

	gen.bind_function('read_virtual_method_through_base_class', 'int', ['base_class &o'])

	gen.finalize()

	return gen.get_output()


test_python = '''\
import my_test

from tests_api import expect_eq

base = my_test.base_class()
expect_eq(base.base_method(), 4)
expect_eq(base.base_method_override(), 4)

derived = my_test.derived_class()
expect_eq(derived.base_method(), 4)  # can still access base class
expect_eq(derived.derived_method(), 8)  # can access its own methods
expect_eq(derived.base_method_override(), 8)  # properly overshadows redeclared base methods

# argument casting through inheritance tree
expect_eq(my_test.read_virtual_method_through_base_class(base), 6)
expect_eq(my_test.read_virtual_method_through_base_class(derived), 9)

# member access through inheritance tree
expect_eq(base.u, 6)
expect_eq(derived.u, 6)  # can access base class member
expect_eq(base.v, 7)
expect_eq(derived.v, 7)  # can access base class static member

expect_eq(base.override, 4)
expect_eq(base.static_override, 1)
expect_eq(derived.override, 12)  # member overshadowing
expect_eq(derived.static_override, 42)  # static member overshadowing
'''
