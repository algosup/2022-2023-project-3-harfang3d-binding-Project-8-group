import os
from os import stat_result
from pypeg2 import parse
import json
import re
import sys
import time
import importlib

import argparse

import gen
import lib

class RustTypeConverterCommon(gen.TypeConverter):
    print("not implemented")
class RustClassTypeDefaultConverter(RustTypeConverterCommon):
    print("not implemented")

    def get_type_glue(self, gen, module_name):
        return ""

class RustGenerator(gen.FABGen):
    default_class_converter = RustClassTypeDefaultConverter

    # kill a bunch of functions we don't care about
    def set_error(self, type, reason):
        return ""

    def get_self(self, ctx):
        return ""

    def get_var(self, i, ctx):
        return ""

    def open_proxy(self, name, max_arg_count, ctx):
        return ""

    def _proto_call(self, self_conv, proto, expr_eval, ctx, fixed_arg_count=None):
        return ""

    def _bind_proxy(self, name, self_conv, protos, desc, expr_eval, ctx, fixed_arg_count=None):
        return ""

    def close_proxy(self, ctx):
        return ""

    def proxy_call_error(self, msg, ctx):
        return ""

    def return_void_from_c(self):
        return ""

    def rval_from_nullptr(self, out_var):
        return ""

    def rval_from_c_ptr(self, conv, out_var, expr, ownership):
        return ""

    def commit_from_c_vars(self, rvals, ctx="default"):
        return ""

    def rbind_function(self, name, rval, args, internal=False):
        return ""