import lib


def bind_test(gen):
	gen.start('my_test')

	lib.bind_defaults(gen)

	# inject test code in the wrapper
	gen.insert_code('''\
struct nested_struct {
	int v{8};
};

struct enclosing_struct {
	nested_struct n;
};
''', True, False)

	nested_struct = gen.begin_class('nested_struct')
	gen.bind_constructor(nested_struct, [])
	gen.bind_member(nested_struct, 'int v')
	gen.end_class(nested_struct)

	enclosing_struct = gen.begin_class('enclosing_struct')
	gen.bind_constructor(enclosing_struct, [])
	gen.bind_member(enclosing_struct, 'nested_struct n')
	gen.end_class(enclosing_struct)

	gen.finalize()
	return gen.get_output()


test_python = '''\
import my_test

#
n = my_test.nested_struct()
assert n.v == 8
n.v -= 4
assert n.v == 4

#
e = my_test.enclosing_struct()
assert e.n.v == 8
e.n.v = 12
assert e.n.v == 12
e.n.v *= 4
assert e.n.v == 48
e.n.v //= 2
assert e.n.v == 24
'''

test_lua = '''\
my_test = require "my_test"

--
n = my_test.nested_struct()
assert(n.v == 8)
n.v = n.v - 4
assert(n.v == 4)

--
e = my_test.enclosing_struct()
assert(e.n.v == 8)
e.n.v = 12
assert(e.n.v == 12)
e.n.v = e.n.v * 4
assert(e.n.v == 48)
e.n.v = e.n.v / 2
assert(e.n.v == 24)
'''

test_go = '''\
package mytest

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

// Test ...
func Test(t *testing.T) {
	n := NewNestedStruct()
	assert.Equal(t, n.GetV(), int32(8), "should be the same.")
	n.SetV(n.GetV() - 4)
	assert.Equal(t, n.GetV(), int32(4), "should be the same.")

	//
	e := NewEnclosingStruct()
	assert.Equal(t, e.GetN().GetV(), int32(8), "should be the same.")
	e.GetN().SetV(12)
	assert.Equal(t, e.GetN().GetV(), int32(12), "should be the same.")
	e.GetN().SetV(e.GetN().GetV() * 4)
	assert.Equal(t, e.GetN().GetV(), int32(48), "should be the same.")
	e.GetN().SetV(e.GetN().GetV() / 2)
	assert.Equal(t, e.GetN().GetV(), int32(24), "should be the same.")
}
'''

test_rust = '''\
include!("bindings.rs");

#[test]
fn test() {
	unsafe {
		let mut n = my_test_constructor_nested_struct();
		assert_eq!(my_test_nested_struct_get_v(n), 8);
		my_test_nested_struct_set_v(n, 4);
		assert_eq!(my_test_nested_struct_get_v(n), 4);

		let mut e = my_test_constructor_enclosing_struct();
		assert_eq!(my_test_nested_struct_get_v(my_test_enclosing_struct_get_n(e)), 8);
		my_test_nested_struct_set_v(my_test_enclosing_struct_get_n(e), 12);
		assert_eq!(my_test_nested_struct_get_v(my_test_enclosing_struct_get_n(e)), 12);
		my_test_nested_struct_set_v(my_test_enclosing_struct_get_n(e), my_test_nested_struct_get_v(my_test_enclosing_struct_get_n(e))* 4);
		assert_eq!(my_test_nested_struct_get_v(my_test_enclosing_struct_get_n(e)), 48);
		my_test_nested_struct_set_v(my_test_enclosing_struct_get_n(e), my_test_nested_struct_get_v(my_test_enclosing_struct_get_n(e)) / 2);
		assert_eq!(my_test_nested_struct_get_v(my_test_enclosing_struct_get_n(e)), 24);
	}
}
'''