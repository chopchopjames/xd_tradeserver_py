%module(directors="1") TradeApi
%{
#include "TradeApi.h"
#include "iconv.h"
%}

%feature("director") TradeSpi;
%include "std_string.i"
%include "TradeApi.h"
%include "TradeInclude.h"


%typemap(out) char[ANY], char[] {
    if ($1) {
        iconv_t cd = iconv_open("utf-8", "gb2312");
        if (cd != reinterpret_cast<iconv_t>(-1)) {
            char buf[4096] = {};
            char **in = &$1;
            char *out = buf;
            size_t inlen = strlen($1), outlen = 4096;

            if (iconv(cd, in, &inlen, &out, &outlen) != static_cast<size_t>(-1))
			{
				size_t size = outlen;
				while (size && (buf[size - 1] == '\0')) --size;
				resultobj = SWIG_FromCharPtrAndSize(buf, size);
			}
            iconv_close(cd);
        }
    }
}

%typemap(in) char *[] {
  /* Check if is a list */
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = (char **) malloc((size+1)*sizeof(char *));
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input, i);
      if (PyString_Check(o)) {
        $1[i] = PyString_AsString(PyList_GetItem($input, i));
      } else {
        free($1);
        PyErr_SetString(PyExc_TypeError, "list must contain strings");
        SWIG_fail;
      }
    }
    $1[i] = 0;
  } else {
    PyErr_SetString(PyExc_TypeError, "not a list");
    SWIG_fail;
  }
}


%extend LoginRequest {
    void set_user_id_python(const char* value) {
        $self->user_id = std::string(value);
    }
}
