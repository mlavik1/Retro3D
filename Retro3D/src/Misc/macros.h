#define getFirst(first, ...) first 
#define getSecond(first, second, ...) second 
#define getAllButFirst(first, ...) EXPAND(__VA_ARGS__)


#define EXPAND(x) x
#define ReturnVaargs(...) EXPAND(__VA_ARGS__)

#define BOOST_PP_VARIADICS 1

#include <boost/preprocessor.hpp>

#define GetAllButFirst(first, ...) EXPAND(__VA_ARGS__)

#define GetDargs(...) GetDargs2(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define GetDargs2(seq) GetDargs3(BOOST_PP_SEQ_HEAD(seq),BOOST_PP_SEQ_TAIL(seq))
#define GetDargs3(name, args) GetDargs6(name,                                \
                              BOOST_PP_SEQ_FOR_EACH_I(GetDargs4,,args), \
                              BOOST_PP_SEQ_FOR_EACH_I(GetDargs5,,args))
#define GetDargs4(rep, data, index, type) (type BOOST_PP_CAT(arg,index))
#define GetDargs5(rep, data, index, type) (BOOST_PP_CAT(arg,index))
#define GetDargs6(name, dargs, cargs)                                   \
  GetDargs8(name, GetDargs7(dargs, void), GetDargs7(cargs, ))
#define GetDargs7(seq, empty)                                           \
  BOOST_PP_IF(BOOST_PP_SEQ_SIZE(seq),                              \
              BOOST_PP_SEQ_TO_TUPLE(seq), (empty))
#define GetDargs8(name, dargs, cargs) dargs


#define GetCargs(...) BOOST_PP_IF(BOOST_PP_IS_EMPTY(GetAllButFirst(__VA_ARGS__)),(),GetCargs2(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)))
#define GetCargs2(seq) GetCargs3(BOOST_PP_SEQ_HEAD(seq),BOOST_PP_SEQ_TAIL(seq))
#define GetCargs3(name, args) GetCargs6(name,                                \
                              BOOST_PP_SEQ_FOR_EACH_I(GetCargs4,,args), \
                              BOOST_PP_SEQ_FOR_EACH_I(GetCargs5,,args))
#define GetCargs4(rep, data, index, type) (type BOOST_PP_CAT(arg,index))
#define GetCargs5(rep, data, index, type) (BOOST_PP_CAT(arg,index))
#define GetCargs6(name, dargs, cargs)                                   \
  GetCargs8(name, GetCargs7(dargs, void), GetCargs7(cargs, ))
#define GetCargs7(seq, empty)                                           \
  BOOST_PP_IF(BOOST_PP_SEQ_SIZE(seq),                              \
              BOOST_PP_SEQ_TO_TUPLE(seq), (empty))
#define GetCargs8(name, dargs, cargs) cargs

/**
* Takes N datatype-arguments, and creates local variables for them on the form: int arg0; float arg1; ...
*/
#define MakeVars(...) MakeVars2(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define MakeVars2(seq) MakeVars3(BOOST_PP_SEQ_HEAD(seq),BOOST_PP_SEQ_TAIL(seq))
#define MakeVars3(name, args) MakeVars6(name,                                \
                              BOOST_PP_SEQ_FOR_EACH_I(MakeVars4,,args), \
                              BOOST_PP_SEQ_FOR_EACH_I(MakeVars5,,args))
#define MakeVars4(rep, data, index, type) type BOOST_PP_CAT(BOOST_PP_CAT(arg,index),;)
#define MakeVars5(rep, data, index, type) (BOOST_PP_CAT(arg,index))
#define MakeVars6(name, dargs, cargs)                                   \
  MakeVars8(name, MakeVars7(dargs, void), MakeVars7(cargs, ))
#define MakeVars7(seq, empty)                                           \
  BOOST_PP_IF(BOOST_PP_SEQ_SIZE((seq)),                              \
              seq, (empty))
#define MakeVars8(name, dargs, cargs) dargs

