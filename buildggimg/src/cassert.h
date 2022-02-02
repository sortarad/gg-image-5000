/******************************************************************************
 *****************************************************************************/

/**************************************************************************//**
 * \file    cassert.h
 * \brief   C compile time assert macros
 *****************************************************************************/

#ifndef CASSERT_H_INCLUDED
#define CASSERT_H_INCLUDED

/** A compile time assertion check.
 *
 *  Validate at compile time that the predicate is true without
 *  generating code. This can be used at any point in a source file
 *  where typedef is legal.
 *
 *  On success, compilation proceeds normally.
 *
 *  On failure, attempts to typedef an array type of negative size. The
 *  offending line will look like
 *      typedef assertion_failed_file_h_42[-1]
 *  where file is the content of the second parameter which should
 *  typically be related in some obvious way to the containing file
 *  name, 42 is the line number in the file on which the assertion
 *  appears, and -1 is the result of a calculation based on the
 *  predicate failing.
 *
 *  \param predicate The predicate to test. It must evaluate to
 *  something that can be coerced to a normal C boolean.
 *
 *  \param file A sequence of legal identifier characters that should
 *  uniquely identify the source file in which this condition appears.
 */

//lint -e(9026, 9022) ignore Function-like macro & unparenthesized macro parameter in definition of macro notes
#define CASSERT(predicate, file) /*lint -e{514} -e{506} -e{9029} -e{9027} -e{9050} */ _impl_CASSERT_LINE(predicate,__LINE__,file)

//lint -e(9023, 9024) ignore '#/##' operator used in macro & Multiple use of '#/##' operators in definition of macro
#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

#endif // CASSERT_H_INCLUDED
