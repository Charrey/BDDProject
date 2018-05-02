/*
 * Copyright 2011-2016 Formal Methods and Tools, University of Twente
 * Copyright 2016 Tom van Dijk, Johannes Kepler University Linz
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * This is an implementation of Multi-Terminal Binary Decision Diagrams.
 * They encode functions on Boolean variables to any domain.
 *
 * Three domains are supported by default: Boolean, Integer and Real.
 * Boolean MTBDDs are identical to BDDs (as supported by the bdd subpackage).
 * Integer MTBDDs are encoded using "int64_t" terminals.
 * Real MTBDDs are encoded using "double" terminals.
 *
 * Labels of Boolean variables of MTBDD nodes are 24-bit integers.
 *
 * Custom terminals are supported.
 *
 * Terminal type "0" is the Integer type, type "1" is the Real type.
 * Type "2" is the Fraction type, consisting of two 32-bit integers (numerator and denominator)
 * For non-Boolean MTBDDs, mtbdd_false is used for partial functions, i.e. mtbdd_false
 * indicates that the function is not defined for a certain input.
 */

/* Do not include this file directly. Instead, include sylvan.h */

#ifndef SYLVAN_MTBDD_H
#define SYLVAN_MTBDD_H

#include <sylvan_mt.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * An MTBDD is a 64-bit value. The low 40 bits are an index into the unique table.
 * The highest 1 bit is the complement edge, indicating negation.
 * For Boolean MTBDDs, this means "not X", for Integer and Real MTBDDs, this means "-X".
 */
typedef uint64_t MTBDD;
typedef MTBDD MTBDDMAP;

/**
 * mtbdd_true is only used in Boolean MTBDDs. mtbdd_false has multiple roles (see above).
 */
#define mtbdd_complement    ((MTBDD)0x8000000000000000LL)
#define mtbdd_false         ((MTBDD)0)
#define mtbdd_true          (mtbdd_false|mtbdd_complement)
#define mtbdd_invalid       ((MTBDD)0xffffffffffffffffLL)

/* Compatibility */
#define BDD                     MTBDD
#define BDDMAP                  MTBDDMAP
#define BDDSET                  MTBDD
#define BDDVAR                  uint32_t
#define sylvan_complement       mtbdd_complement
#define sylvan_false            mtbdd_false
#define sylvan_true             mtbdd_true
#define sylvan_invalid          mtbdd_invalid
#define sylvan_init_bdd         sylvan_init_mtbdd
#define sylvan_ref              mtbdd_ref
#define sylvan_deref            mtbdd_deref
#define sylvan_count_refs       mtbdd_count_refs
#define sylvan_protect          mtbdd_protect
#define sylvan_unprotect        mtbdd_unprotect
#define sylvan_count_protected  mtbdd_count_protected
#define sylvan_gc_mark_rec      mtbdd_gc_mark_rec
#define sylvan_notify_ondead    mtbdd_notify_ondead
#define bdd_refs_push           mtbdd_refs_push
#define bdd_refs_pop            mtbdd_refs_pop
#define bdd_refs_spawn          mtbdd_refs_spawn
#define bdd_refs_sync           mtbdd_refs_sync
#define sylvan_map_empty        mtbdd_map_empty
#define sylvan_map_isempty      mtbdd_map_isempty
#define sylvan_map_key          mtbdd_map_key
#define sylvan_map_value        mtbdd_map_value
#define sylvan_map_next         mtbdd_map_next
#define sylvan_map_contains     mtbdd_map_contains
#define sylvan_map_count        mtbdd_map_count
#define sylvan_map_add          mtbdd_map_add
#define sylvan_map_addall       mtbdd_map_addall
#define sylvan_map_remove       mtbdd_map_remove
#define sylvan_map_removeall    mtbdd_map_removeall
#define sylvan_set_empty        mtbdd_set_empty
#define sylvan_set_isempty      mtbdd_set_isempty
#define sylvan_set_add          mtbdd_set_add
#define sylvan_set_addall       mtbdd_set_addall
#define sylvan_set_remove       mtbdd_set_remove
#define sylvan_set_removeall    mtbdd_set_removeall
#define sylvan_set_first        mtbdd_set_first
#define sylvan_set_next         mtbdd_set_next
#define sylvan_set_fromarray    mtbdd_set_fromarray
#define sylvan_set_toarray      mtbdd_set_toarray
#define sylvan_set_in           mtbdd_set_in
#define sylvan_set_count        mtbdd_set_count
#define sylvan_test_isset       mtbdd_test_isset
#define sylvan_var              mtbdd_getvar
#define sylvan_low              mtbdd_getlow
#define sylvan_high             mtbdd_gethigh
#define sylvan_makenode         mtbdd_makenode
#define sylvan_makemapnode      mtbdd_makemapnode
#define sylvan_support          mtbdd_support
#define sylvan_test_isbdd       mtbdd_test_isvalid
#define sylvan_nodecount        mtbdd_nodecount
#define sylvan_printdot         mtbdd_printdot
#define sylvan_fprintdot        mtbdd_fprintdot
#define sylvan_printsha         mtbdd_printsha
#define sylvan_fprintsha        mtbdd_fprintsha
#define sylvan_getsha           mtbdd_getsha

/**
 * Initialize MTBDD functionality.
 * This initializes internal and external referencing datastructures,
 * and registers them in the garbage collection framework.
 */
void sylvan_init_mtbdd(void);

/**
 * Create a MTBDD terminal of type <type> and value <value>.
 * For custom types, the value could be a pointer to some external struct.
 */
MTBDD mtbdd_makeleaf(uint32_t type, uint64_t value);

/**
 * Create an internal MTBDD node of Boolean variable <var>, with low edge <low> and high edge <high>.
 * <var> is a 24-bit integer.
 * Please note that this does NOT check variable ordering!
 */
MTBDD _mtbdd_makenode(uint32_t var, MTBDD low, MTBDD high);
static inline MTBDD mtbdd_makenode(uint32_t var, MTBDD low, MTBDD high)
{
    return low == high ? low : _mtbdd_makenode(var, low, high);
}

/**
 * Returns 1 is the MTBDD is a terminal, or 0 otherwise.
 */
int mtbdd_isleaf(MTBDD mtbdd);
#define mtbdd_isnode(mtbdd) (mtbdd_isleaf(mtbdd) ? 0 : 1)

/**
 * For MTBDD terminals, returns <type> and <value>
 */
uint32_t mtbdd_gettype(MTBDD terminal);
uint64_t mtbdd_getvalue(MTBDD terminal);

/**
 * For internal MTBDD nodes, returns <var>, <low> and <high>
 */
uint32_t mtbdd_getvar(MTBDD node);
MTBDD mtbdd_getlow(MTBDD node);
MTBDD mtbdd_gethigh(MTBDD node);

/**
 * Compute the complement of the MTBDD.
 * For Boolean MTBDDs, this means "not X".
 */
#define mtbdd_hascomp(dd) ((dd & mtbdd_complement) ? 1 : 0)
#define mtbdd_comp(dd) (dd ^ mtbdd_complement)
#define mtbdd_not(dd) (dd ^ mtbdd_complement)

/**
 * Create terminals representing int64_t (type 0), double (type 1), or fraction (type 2) values
 */
MTBDD mtbdd_int64(int64_t value);
MTBDD mtbdd_double(double value);
MTBDD mtbdd_fraction(int64_t numer, uint64_t denom);

/**
 * Get the value of a terminal (for Integer, Real and Fraction terminals, types 0, 1 and 2)
 */
int64_t mtbdd_getint64(MTBDD terminal);
double mtbdd_getdouble(MTBDD terminal);
#define mtbdd_getnumer(terminal) ((int32_t)(mtbdd_getvalue(terminal)>>32))
#define mtbdd_getdenom(terminal) ((uint32_t)(mtbdd_getvalue(terminal)&0xffffffff))

/**
 * Create the conjunction of variables in arr,
 * i.e. arr[0] \and arr[1] \and ... \and arr[length-1]
 * The variable in arr must be ordered.
 */
MTBDD mtbdd_fromarray(uint32_t* arr, size_t length);

/**
 * Given a cube of variables, write each variable to arr.
 * WARNING: arr must be sufficiently long!
 */
void mtbdd_toarray(MTBDD set, uint32_t *arr);

/**
 * Create a MTBDD cube representing the conjunction of variables in their positive or negative
 * form depending on whether the cube[idx] equals 0 (negative), 1 (positive) or 2 (any).
 * Use cube[idx]==3 for "s=s'" in interleaved variables (matches with next variable)
 * <variables> is the cube of variables (var1 \and var2 \and ... \and varn)
 */
MTBDD mtbdd_cube(MTBDD variables, uint8_t *cube, MTBDD terminal);

/**
 * Same as mtbdd_cube, but extends <mtbdd> with the assignment <cube> \to <terminal>.
 * If <mtbdd> already assigns a value to the cube, the new value <terminal> is taken.
 * Does not support cube[idx]==3.
 */
#define mtbdd_union_cube(mtbdd, variables, cube, terminal) CALL(mtbdd_union_cube, mtbdd, variables, cube, terminal)
TASK_DECL_4(BDD, mtbdd_union_cube, MTBDD, MTBDD, uint8_t*, MTBDD);

/**
 * Count the number of satisfying assignments (minterms) leading to a non-false leaf
 */
TASK_DECL_2(double, mtbdd_satcount, MTBDD, size_t);
#define mtbdd_satcount(dd, nvars) CALL(mtbdd_satcount, dd, nvars)

/**
 * Count the number of MTBDD leaves (excluding mtbdd_false and mtbdd_true) in the given <count> MTBDDs
 */
size_t mtbdd_leafcount_more(const MTBDD *mtbdds, size_t count);
#define mtbdd_leafcount(dd) mtbdd_leafcount_more(&dd, 1)

/**
 * Count the number of MTBDD nodes and terminals (excluding mtbdd_false and mtbdd_true) in the given <count> MTBDDs
 */
size_t mtbdd_nodecount_more(const MTBDD *mtbdds, size_t count);

static inline size_t
mtbdd_nodecount(const MTBDD dd) {
    return mtbdd_nodecount_more(&dd, 1);
}

/**
 * Callback function types for binary ("dyadic") and unary ("monadic") operations.
 * The callback function returns either the MTBDD that is the result of applying op to the MTBDDs,
 * or mtbdd_invalid if op cannot be applied.
 * The binary function may swap the two parameters (if commutative) to improve caching.
 * The unary function is allowed an extra parameter (be careful of caching)
 */
LACE_TYPEDEF_CB(MTBDD, mtbdd_apply_op, MTBDD*, MTBDD*);
LACE_TYPEDEF_CB(MTBDD, mtbdd_applyp_op, MTBDD*, MTBDD*, size_t);
LACE_TYPEDEF_CB(MTBDD, mtbdd_uapply_op, MTBDD, size_t);

/**
 * Apply a binary operation <op> to <a> and <b>.
 * Callback <op> is consulted before the cache, thus the application to terminals is not cached.
 */
TASK_DECL_3(MTBDD, mtbdd_apply, MTBDD, MTBDD, mtbdd_apply_op);
#define mtbdd_apply(a, b, op) CALL(mtbdd_apply, a, b, op)

/**
 * Apply a binary operation <op> with id <opid> to <a> and <b> with parameter <p>
 * Callback <op> is consulted before the cache, thus the application to terminals is not cached.
 */
TASK_DECL_5(MTBDD, mtbdd_applyp, MTBDD, MTBDD, size_t, mtbdd_applyp_op, uint64_t);
#define mtbdd_applyp(a, b, p, op, opid) CALL(mtbdd_applyp, a, b, p, op, opid)

/**
 * Apply a unary operation <op> to <dd>.
 * Callback <op> is consulted after the cache, thus the application to a terminal is cached.
 */
TASK_DECL_3(MTBDD, mtbdd_uapply, MTBDD, mtbdd_uapply_op, size_t);
#define mtbdd_uapply(dd, op, param) CALL(mtbdd_uapply, dd, op, param)

/**
 * Callback function types for abstraction.
 * MTBDD mtbdd_abstract_op(MTBDD a, MTBDD b, int k).
 * The function is either called with k==0 (apply to two arguments) or k>0 (k skipped BDD variables)
 * k == 0  =>  res := apply op to a and b
 * k  > 0  =>  res := apply op to op(a, a, k-1) and op(a, a, k-1)
 */
LACE_TYPEDEF_CB(MTBDD, mtbdd_abstract_op, MTBDD, MTBDD, int);

/**
 * Abstract the variables in <v> from <a> using the binary operation <op>.
 */
TASK_DECL_3(MTBDD, mtbdd_abstract, MTBDD, MTBDD, mtbdd_abstract_op);
#define mtbdd_abstract(a, v, op) CALL(mtbdd_abstract, a, v, op)

/**
 * Unary operation Negate.
 * Supported domains: Integer, Real, Fraction
 */
TASK_DECL_2(MTBDD, mtbdd_op_negate, MTBDD, size_t);

/**
 * Binary operation Plus (for MTBDDs of same type)
 * Only for MTBDDs where either all leaves are Boolean, or Integer, or Double.
 * For Integer/Double MTBDDs, mtbdd_false is interpreted as "0" or "0.0".
 */
TASK_DECL_2(MTBDD, mtbdd_op_plus, MTBDD*, MTBDD*);
TASK_DECL_3(MTBDD, mtbdd_abstract_op_plus, MTBDD, MTBDD, int);

/**
 * Binary operation Minus (for MTBDDs of same type)
 * Only for MTBDDs where either all leaves are Boolean, or Integer, or Double.
 * For Integer/Double MTBDDs, mtbdd_false is interpreted as "0" or "0.0".
 */
TASK_DECL_2(MTBDD, mtbdd_op_minus, MTBDD*, MTBDD*);

/**
 * Binary operation Times (for MTBDDs of same type)
 * Only for MTBDDs where either all leaves are Boolean, or Integer, or Double.
 * For Integer/Double MTBDD, if either operand is mtbdd_false (not defined),
 * then the result is mtbdd_false (i.e. not defined).
 */
TASK_DECL_2(MTBDD, mtbdd_op_times, MTBDD*, MTBDD*);
TASK_DECL_3(MTBDD, mtbdd_abstract_op_times, MTBDD, MTBDD, int);

/**
 * Binary operation Minimum (for MTBDDs of same type)
 * Only for MTBDDs where either all leaves are Boolean, or Integer, or Double.
 * For Integer/Double MTBDD, if either operand is mtbdd_false (not defined),
 * then the result is the other operand.
 */
TASK_DECL_2(MTBDD, mtbdd_op_min, MTBDD*, MTBDD*);
TASK_DECL_3(MTBDD, mtbdd_abstract_op_min, MTBDD, MTBDD, int);

/**
 * Binary operation Maximum (for MTBDDs of same type)
 * Only for MTBDDs where either all leaves are Boolean, or Integer, or Double.
 * For Integer/Double MTBDD, if either operand is mtbdd_false (not defined),
 * then the result is the other operand.
 */
TASK_DECL_2(MTBDD, mtbdd_op_max, MTBDD*, MTBDD*);
TASK_DECL_3(MTBDD, mtbdd_abstract_op_max, MTBDD, MTBDD, int);

/**
 * Compute -a
 */
#define mtbdd_negate(a) mtbdd_uapply(a, TASK(mtbdd_op_negate), 0)

/**
 * Compute a + b
 */
#define mtbdd_plus(a, b) mtbdd_apply(a, b, TASK(mtbdd_op_plus))

/**
 * Compute a - b
 */
#define mtbdd_minus(a, b) mtbdd_apply(a, b, TASK(mtbdd_op_minus))

/**
 * Compute a * b
 */
#define mtbdd_times(a, b) mtbdd_apply(a, b, TASK(mtbdd_op_times))

/**
 * Compute min(a, b)
 */
#define mtbdd_min(a, b) mtbdd_apply(a, b, TASK(mtbdd_op_min))

/**
 * Compute max(a, b)
 */
#define mtbdd_max(a, b) mtbdd_apply(a, b, TASK(mtbdd_op_max))

/**
 * Abstract the variables in <v> from <a> by taking the sum of all values
 */
#define mtbdd_abstract_plus(dd, v) mtbdd_abstract(dd, v, TASK(mtbdd_abstract_op_plus))

/**
 * Abstract the variables in <v> from <a> by taking the product of all values
 */
#define mtbdd_abstract_times(dd, v) mtbdd_abstract(dd, v, TASK(mtbdd_abstract_op_times))

/**
 * Abstract the variables in <v> from <a> by taking the minimum of all values
 */
#define mtbdd_abstract_min(dd, v) mtbdd_abstract(dd, v, TASK(mtbdd_abstract_op_min))

/**
 * Abstract the variables in <v> from <a> by taking the maximum of all values
 */
#define mtbdd_abstract_max(dd, v) mtbdd_abstract(dd, v, TASK(mtbdd_abstract_op_max))

/**
 * Compute IF <f> THEN <g> ELSE <h>.
 * <f> must be a Boolean MTBDD (or standard BDD).
 */
TASK_DECL_3(MTBDD, mtbdd_ite, MTBDD, MTBDD, MTBDD);
#define mtbdd_ite(f, g, h) CALL(mtbdd_ite, f, g, h);

/**
 * Multiply <a> and <b>, and abstract variables <vars> using summation.
 * This is similar to the "and_exists" operation in BDDs.
 */
TASK_DECL_3(MTBDD, mtbdd_and_abstract_plus, MTBDD, MTBDD, MTBDD);
#define mtbdd_and_abstract_plus(a, b, vars) CALL(mtbdd_and_abstract_plus, a, b, vars)
#define mtbdd_and_exists mtbdd_and_abstract_plus

/**
 * Multiply <a> and <b>, and abstract variables <vars> by taking the maximum.
 */
TASK_DECL_3(MTBDD, mtbdd_and_abstract_max, MTBDD, MTBDD, MTBDD);
#define mtbdd_and_abstract_max(a, b, vars) CALL(mtbdd_and_abstract_max, a, b, vars)

/**
 * Monad that converts double to a Boolean MTBDD, translate terminals >= value to 1 and to 0 otherwise;
 */
TASK_DECL_2(MTBDD, mtbdd_op_threshold_double, MTBDD, size_t)

/**
 * Monad that converts double to a Boolean MTBDD, translate terminals > value to 1 and to 0 otherwise;
 */
TASK_DECL_2(MTBDD, mtbdd_op_strict_threshold_double, MTBDD, size_t)

/**
 * Convert double to a Boolean MTBDD, translate terminals >= value to 1 and to 0 otherwise;
 */
TASK_DECL_2(MTBDD, mtbdd_threshold_double, MTBDD, double);
#define mtbdd_threshold_double(dd, value) CALL(mtbdd_threshold_double, dd, value)

/**
 * Convert double to a Boolean MTBDD, translate terminals > value to 1 and to 0 otherwise;
 */
TASK_DECL_2(MTBDD, mtbdd_strict_threshold_double, MTBDD, double);
#define mtbdd_strict_threshold_double(dd, value) CALL(mtbdd_strict_threshold_double, dd, value)

/**
 * For two Double MTBDDs, calculate whether they are equal module some value epsilon
 * i.e. abs(a-b) < e
 */
TASK_DECL_3(MTBDD, mtbdd_equal_norm_d, MTBDD, MTBDD, double);
#define mtbdd_equal_norm_d(a, b, epsilon) CALL(mtbdd_equal_norm_d, a, b, epsilon)

/**
 * For two Double MTBDDs, calculate whether they are equal modulo some value epsilon
 * This version computes the relative difference vs the value in a.
 * i.e. abs((a-b)/a) < e
 */
TASK_DECL_3(MTBDD, mtbdd_equal_norm_rel_d, MTBDD, MTBDD, double);
#define mtbdd_equal_norm_rel_d(a, b, epsilon) CALL(mtbdd_equal_norm_rel_d, a, b, epsilon)

/**
 * For two MTBDDs a, b, return mtbdd_true if all common assignments a(s) <= b(s), mtbdd_false otherwise.
 * For domains not in a / b, assume True.
 */
TASK_DECL_2(MTBDD, mtbdd_leq, MTBDD, MTBDD);
#define mtbdd_leq(a, b) CALL(mtbdd_leq, a, b)

/**
 * For two MTBDDs a, b, return mtbdd_true if all common assignments a(s) < b(s), mtbdd_false otherwise.
 * For domains not in a / b, assume True.
 */
TASK_DECL_2(MTBDD, mtbdd_less, MTBDD, MTBDD);
#define mtbdd_less(a, b) CALL(mtbdd_less, a, b)

/**
 * For two MTBDDs a, b, return mtbdd_true if all common assignments a(s) >= b(s), mtbdd_false otherwise.
 * For domains not in a / b, assume True.
 */
TASK_DECL_2(MTBDD, mtbdd_geq, MTBDD, MTBDD);
#define mtbdd_geq(a, b) CALL(mtbdd_geq, a, b)

/**
 * For two MTBDDs a, b, return mtbdd_true if all common assignments a(s) > b(s), mtbdd_false otherwise.
 * For domains not in a / b, assume True.
 */
TASK_DECL_2(MTBDD, mtbdd_greater, MTBDD, MTBDD);
#define mtbdd_greater(a, b) CALL(mtbdd_greater, a, b)

/**
 * Calculate the support of a MTBDD, i.e. the cube of all variables that appear in the MTBDD nodes.
 */
TASK_DECL_1(MTBDD, mtbdd_support, MTBDD);
#define mtbdd_support(dd) CALL(mtbdd_support, dd)

/**
 * Function composition, for each node with variable <key> which has a <key,value> pair in <map>,
 * replace the node by the result of mtbdd_ite(<value>, <low>, <high>).
 * Each <value> in <map> must be a Boolean MTBDD.
 */
TASK_DECL_2(MTBDD, mtbdd_compose, MTBDD, MTBDDMAP);
#define mtbdd_compose(dd, map) CALL(mtbdd_compose, dd, map)

/**
 * Compute minimal leaf in the MTBDD (for Integer, Double, Rational MTBDDs)
 */
TASK_DECL_1(MTBDD, mtbdd_minimum, MTBDD);
#define mtbdd_minimum(dd) CALL(mtbdd_minimum, dd)

/**
 * Compute maximal leaf in the MTBDD (for Integer, Double, Rational MTBDDs)
 */
TASK_DECL_1(MTBDD, mtbdd_maximum, MTBDD);
#define mtbdd_maximum(dd) CALL(mtbdd_maximum, dd)

/**
 * Given a MTBDD <dd> and a cube of variables <variables> expected in <dd>,
 * mtbdd_enum_first and mtbdd_enum_next enumerates the unique paths in <dd> that lead to a non-False leaf.
 * 
 * The function returns the leaf (or mtbdd_false if no new path is found) and encodes the path
 * in the supplied array <arr>: 0 for a low edge, 1 for a high edge, and 2 if the variable is skipped.
 *
 * The supplied array <arr> must be large enough for all variables in <variables>.
 *
 * Usage:
 * MTBDD leaf = mtbdd_enum_first(dd, variables, arr, NULL);
 * while (leaf != mtbdd_false) {
 *     .... // do something with arr/leaf
 *     leaf = mtbdd_enum_next(dd, variables, arr, NULL);
 * }
 *
 * The callback is an optional function that returns 0 when the given terminal node should be skipped.
 */
typedef int (*mtbdd_enum_filter_cb)(MTBDD);
MTBDD mtbdd_enum_first(MTBDD dd, MTBDD variables, uint8_t *arr, mtbdd_enum_filter_cb filter_cb);
MTBDD mtbdd_enum_next(MTBDD dd, MTBDD variables, uint8_t *arr, mtbdd_enum_filter_cb filter_cb);

/**
 * Given an MTBDD <dd> and a cube of variables <variables> expected in <dd>,
 * mtbdd_enum_all_first and mtbdd_enum_all_next enumerate all satisfying assignments in <dd> that lead
 * to a non-False leaf.
 *
 * The functions return the leaf (or mtbdd_false if no new satisfying assignment is found) and encodes
 * the assignment in the supplied array <arr>, 0 for False and 1 for True.
 *
 * The supplied array <arr> must be large enough for all variables in <variables>.
 *
 * Usage:
 * MTBDD leaf = mtbdd_enum_first(dd, variables, arr, NULL);
 * while (leaf != mtbdd_false) {
 *     .... // do something with arr/leaf
 *     leaf = mtbdd_enum_next(dd, variables, arr, NULL);
 * }
 *
 * The callback is an optional function that returns 0 when the given terminal node should be skipped.
 */
MTBDD mtbdd_enum_all_first(MTBDD dd, MTBDD variables, uint8_t *arr, mtbdd_enum_filter_cb filter_cb);
MTBDD mtbdd_enum_all_next(MTBDD dd, MTBDD variables, uint8_t *arr, mtbdd_enum_filter_cb filter_cb);

/**
 * Given a MTBDD <dd>, call <cb> with context <context> for every unique path in <dd> ending in leaf <leaf>.
 *
 * Usage:
 * VOID_TASK_3(cb, mtbdd_enum_trace_t, trace, MTBDD, leaf, void*, context) { ... do something ... }
 * mtbdd_enum_par(dd, cb, context);
 */
typedef struct mtbdd_enum_trace {
    struct mtbdd_enum_trace *prev;
    uint32_t var;
    int val;  // 0 or 1
} * mtbdd_enum_trace_t;

LACE_TYPEDEF_CB(void, mtbdd_enum_cb, mtbdd_enum_trace_t, MTBDD, void*)
VOID_TASK_DECL_3(mtbdd_enum_par, MTBDD, mtbdd_enum_cb, void*);
#define mtbdd_enum_par(dd, cb, context) CALL(mtbdd_enum_par, dd, cb, context)

/**
 * Function composition after partial evaluation.
 *
 * Given a function F(X) = f, compute the composition F'(X) = g(f) for every assignment to X.
 * All variables X in <vars> must appear before all variables in f and g(f).
 *
 * Usage:
 * TASK_2(MTBDD, g, MTBDD, in) { ... return g of <in> ... }
 * MTBDD x_vars = ...;  // the cube of variables x
 * MTBDD result = mtbdd_eval_compose(dd, x_vars, TASK(g));
 */
LACE_TYPEDEF_CB(MTBDD, mtbdd_eval_compose_cb, MTBDD);
TASK_DECL_3(MTBDD, mtbdd_eval_compose, MTBDD, MTBDD, mtbdd_eval_compose_cb);
#define mtbdd_eval_compose(dd, vars, cb) CALL(mtbdd_eval_compose, dd, vars, cb)

/**
 * For debugging.
 * Tests if all nodes in the MTBDD are correctly ``marked'' in the nodes table.
 * Tests if variables in the internal nodes appear in-order.
 * In Debug mode, this will cause assertion failures instead of returning 0.
 * Returns 1 if all is fine, or 0 otherwise.
 */
TASK_DECL_1(int, mtbdd_test_isvalid, MTBDD);
#define mtbdd_test_isvalid(mtbdd) CALL(mtbdd_test_isvalid, mtbdd)

/**
 * Write a .dot representation of a given MTBDD
 * The callback function is required for custom terminals.
 */
void mtbdd_fprintdot(FILE *out, MTBDD mtbdd);
#define mtbdd_printdot(mtbdd, cb) mtbdd_fprintdot(stdout, mtbdd)

/**
 * Write a .dot representation of a given MTBDD, but without complement edges.
 */
void mtbdd_fprintdot_nc(FILE *out, MTBDD mtbdd);
#define mtbdd_printdot_nc(mtbdd, cb) mtbdd_fprintdot_nc(stdout, mtbdd)

/**
 * Write a text representation of a leaf to the given file.
 */
void mtbdd_fprint_leaf(FILE *out, MTBDD leaf);

/**
 * Write a text representation of a leaf to stdout.
 */
void mtbdd_print_leaf(MTBDD leaf);

/**
 * Obtain the textual representation of a leaf.
 * The returned result is either equal to the given <buf> (if the results fits)
 * or to a newly allocated array (with malloc).
 */
char *mtbdd_leaf_to_str(MTBDD leaf, char *buf, size_t buflen);

/**
 * Some debugging functions that generate SHA2 hashes of MTBDDs.
 * They are independent of where nodes are located in hash tables.
 * Note that they are not "perfect", but they can be useful to run easy sanity checks.
 */

/**
 * Print SHA2 hash to stdout.
 */
void mtbdd_printsha(MTBDD dd);

/**
 * Print SHA2 hash to given file.
 */
void mtbdd_fprintsha(FILE *f, MTBDD dd);

/**
 * Obtain SHA2 hash; target array must be at least 65 bytes long.
 */
void mtbdd_getsha(MTBDD dd, char *target);

/**
 * Visitor functionality for MTBDDs.
 * Visits internal nodes and leafs.
 */

/**
 * pre_cb callback: given input MTBDD and context,
 *                  return whether to visit children (if not leaf)
 * post_cb callback: given input MTBDD and context
 */
LACE_TYPEDEF_CB(int, mtbdd_visit_pre_cb, MTBDD, void*);
LACE_TYPEDEF_CB(void, mtbdd_visit_post_cb, MTBDD, void*);

/**
 * Sequential visit operation
 */
VOID_TASK_DECL_4(mtbdd_visit_seq, MTBDD, mtbdd_visit_pre_cb, mtbdd_visit_post_cb, void*);
#define mtbdd_visit_seq(...) CALL(mtbdd_visit_seq, __VA_ARGS__)

/**
 * Parallel visit operation
 */
VOID_TASK_DECL_4(mtbdd_visit_par, MTBDD, mtbdd_visit_pre_cb, mtbdd_visit_post_cb, void*);
#define mtbdd_visit_par(...) CALL(mtbdd_visit_par, __VA_ARGS__)

/**
 * Writing MTBDDs to file.
 *
 * Every node that is to be written is assigned a number, starting from 1,
 * such that reading the result in the future can be done in one pass.
 *
 * We use a skiplist to store the assignment.
 *
 * The functions mtbdd_writer_tobinary and mtbdd_writer_totext can be used to
 * store an array of MTBDDs to binary format or text format.
 *
 * One could also do the procedure manually instead.
 * - call mtbdd_writer_start to allocate the skiplist.
 * - call mtbdd_writer_add to add a given MTBDD to the skiplist
 * - call mtbdd_writer_writebinary to write all added nodes to a file
 * - OR:  mtbdd_writer_writetext to write all added nodes in text format
 * - call mtbdd_writer_get to obtain the MTBDD identifier as stored in the skiplist
 * - call mtbdd_writer_end to free the skiplist
 */

/**
 * Write <count> decision diagrams given in <dds> in internal binary form to <file>.
 *
 * The internal binary format is as follows, to store <count> decision diagrams...
 * uint64_t: nodecount -- number of nodes
 * <nodecount> times uint128_t: each leaf/node
 * uint64_t: count -- number of stored decision diagrams
 * <count> times uint64_t: each stored decision diagram
 */
VOID_TASK_DECL_3(mtbdd_writer_tobinary, FILE *, MTBDD *, int);
#define mtbdd_writer_tobinary(file, dds, count) CALL(mtbdd_writer_tobinary, file, dds, count)

/**
 * Write <count> decision diagrams given in <dds> in ASCII form to <file>.
 * Also supports custom leaves using the leaf_to_str callback.
 *
 * The text format writes in the same order as the binary format, except...
 * [
 *   node(id, var, low, high), -- for a normal node (no complement on high)
 *   node(id, var, low, ~high), -- for a normal node (complement on high)
 *   leaf(id, type, "value"), -- for a leaf (with value between "")
 * ],[dd1, dd2, dd3, ...,] -- and each the stored decision diagram.
 */

VOID_TASK_DECL_3(mtbdd_writer_totext, FILE *, MTBDD *, int);
#define mtbdd_writer_totext(file, dds, count) CALL(mtbdd_writer_totext, file, dds, count)

/**
 * Skeleton typedef for the skiplist
 */
typedef struct sylvan_skiplist *sylvan_skiplist_t;

/**
 * Allocate a skiplist for writing an MTBDD.
 */
sylvan_skiplist_t mtbdd_writer_start(void);

/**
 * Add the given MTBDD to the skiplist.
 */
VOID_TASK_DECL_2(mtbdd_writer_add, sylvan_skiplist_t, MTBDD);
#define mtbdd_writer_add(sl, dd) CALL(mtbdd_writer_add, sl, dd)

/**
 * Write all assigned MTBDD nodes in binary format to the file.
 */
void mtbdd_writer_writebinary(FILE *out, sylvan_skiplist_t sl);

/**
 * Retrieve the identifier of the given stored MTBDD.
 * This is useful if you want to be able to retrieve the stored MTBDD later.
 */
uint64_t mtbdd_writer_get(sylvan_skiplist_t sl, MTBDD dd);

/**
 * Free the allocated skiplist.
 */
void mtbdd_writer_end(sylvan_skiplist_t sl);

/**
 * Reading MTBDDs from file.
 *
 * The function mtbdd_reader_frombinary is basically the reverse of mtbdd_writer_tobinary.
 *
 * One can also perform the procedure manually.
 * - call mtbdd_reader_readbinary to read the nodes from file
 * - call mtbdd_reader_get to obtain the MTBDD for the given identifier as stored in the file.
 * - call mtbdd_reader_end to free the array returned by mtbdd_reader_readbinary
 *
 * Returns 0 if successful, -1 otherwise.
 */

/*
 * Read <count> decision diagrams to <dds> from <file> in internal binary form.
 */
TASK_DECL_3(int, mtbdd_reader_frombinary, FILE*, MTBDD*, int);
#define mtbdd_reader_frombinary(file, dds, count) CALL(mtbdd_reader_frombinary, file, dds, count)

/**
 * Reading a file earlier written with mtbdd_writer_writebinary
 * Returns an array with the conversion from stored identifier to MTBDD
 * This array is allocated with malloc and must be freed afterwards.
 * Returns NULL if there was an error.
 */

TASK_DECL_1(uint64_t*, mtbdd_reader_readbinary, FILE*);
#define mtbdd_reader_readbinary(file) CALL(mtbdd_reader_readbinary, file)

/**
 * Retrieve the MTBDD of the given stored identifier.
 */
MTBDD mtbdd_reader_get(uint64_t* arr, uint64_t identifier);

/**
 * Free the allocated translation array
 */
void mtbdd_reader_end(uint64_t *arr);

/**
 * MTBDDSET
 * Just some convenience functions for handling sets of variables represented as a 
 * cube (conjunction) of positive literals
 */
#define mtbdd_set_empty()                   mtbdd_true
#define mtbdd_set_isempty(set)              (set == mtbdd_true)
#define mtbdd_set_add(set, var)             sylvan_and(set, sylvan_ithvar(var))
#define mtbdd_set_addall(set, set2)         sylvan_and(set, set2)
#define mtbdd_set_remove(set, var)          sylvan_exists(set, var)
#define mtbdd_set_removeall(set, set2)      sylvan_exists(set, set2)
#define mtbdd_set_first(set)                sylvan_var(set)
#define mtbdd_set_next(set)                 sylvan_high(set)
#define mtbdd_set_fromarray(arr, count)     mtbdd_fromarray(arr, count)
#define mtbdd_set_toarray(set, arr)         mtbdd_toarray(set, arr)
int mtbdd_set_in(BDDSET set, BDDVAR var);
size_t mtbdd_set_count(BDDSET set);
void mtbdd_test_isset(BDDSET set);

/**
 * MTBDDMAP, maps uint32_t variables to MTBDDs.
 * A MTBDDMAP node has variable level, low edge going to the next MTBDDMAP, high edge to the mapped MTBDD.
 */
#define mtbdd_map_empty() mtbdd_false
#define mtbdd_map_isempty(map) (map == mtbdd_false ? 1 : 0)
#define mtbdd_map_key(map) mtbdd_getvar(map)
#define mtbdd_map_value(map) mtbdd_gethigh(map)
#define mtbdd_map_next(map) mtbdd_getlow(map)

/**
 * Return 1 if the map contains the key, 0 otherwise.
 */
int mtbdd_map_contains(MTBDDMAP map, uint32_t key);

/**
 * Retrieve the number of keys in the map.
 */
size_t mtbdd_map_count(MTBDDMAP map);

/**
 * Add the pair <key,value> to the map, overwrites if key already in map.
 */
MTBDDMAP mtbdd_map_add(MTBDDMAP map, uint32_t key, MTBDD value);

/**
 * Add all values from map2 to map1, overwrites if key already in map1.
 */
MTBDDMAP mtbdd_map_addall(MTBDDMAP map1, MTBDDMAP map2);

/**
 * Remove the key <key> from the map and return the result
 */
MTBDDMAP mtbdd_map_remove(MTBDDMAP map, uint32_t key);

/**
 * Remove all keys in the cube <variables> from the map and return the result
 */
MTBDDMAP mtbdd_map_removeall(MTBDDMAP map, MTBDD variables);

/**
 * Garbage collection
 * Sylvan supplies two default methods to handle references to nodes, but the user
 * is encouraged to implement custom handling. Simply add a handler using sylvan_gc_add_mark
 * and let the handler call mtbdd_gc_mark_rec for every MTBDD that should be saved
 * during garbage collection.
 */

/**
 * Call mtbdd_gc_mark_rec for every mtbdd you want to keep in your custom mark functions.
 */
VOID_TASK_DECL_1(mtbdd_gc_mark_rec, MTBDD);
#define mtbdd_gc_mark_rec(mtbdd) CALL(mtbdd_gc_mark_rec, mtbdd)

/**
 * Default external referencing. During garbage collection, MTBDDs marked with mtbdd_ref will
 * be kept in the forest.
 * It is recommended to prefer mtbdd_protect and mtbdd_unprotect.
 */
MTBDD mtbdd_ref(MTBDD a);
void mtbdd_deref(MTBDD a);
size_t mtbdd_count_refs(void);

/**
 * Default external pointer referencing. During garbage collection, the pointers are followed and the MTBDD
 * that they refer to are kept in the forest.
 */
void mtbdd_protect(MTBDD* ptr);
void mtbdd_unprotect(MTBDD* ptr);
size_t mtbdd_count_protected(void);

/**
 * If mtbdd_set_ondead is set to a callback, then this function marks MTBDDs (terminals).
 * When they are dead after the mark phase in garbage collection, the callback is called for marked MTBDDs.
 * The ondead callback can either perform cleanup or resurrect dead terminals.
 */
#define mtbdd_notify_ondead(dd) llmsset_notify_ondead(nodes, dd&~mtbdd_complement)

/**
 * Infrastructure for internal references (per-thread, e.g. during MTBDD operations)
 * Use mtbdd_refs_push and mtbdd_refs_pop to put MTBDDs on a thread-local reference stack.
 * Use mtbdd_refs_spawn and mtbdd_refs_sync around SPAWN and SYNC operations when the result
 * of the spawned Task is a MTBDD that must be kept during garbage collection.
 */
typedef struct mtbdd_refs_internal
{
    size_t r_size, r_count;
    size_t s_size, s_count;
    MTBDD *results;
    Task **spawns;
} *mtbdd_refs_internal_t;

extern DECLARE_THREAD_LOCAL(mtbdd_refs_key, mtbdd_refs_internal_t);

static inline MTBDD
mtbdd_refs_push(MTBDD mtbdd)
{
    LOCALIZE_THREAD_LOCAL(mtbdd_refs_key, mtbdd_refs_internal_t);
    if (mtbdd_refs_key->r_count >= mtbdd_refs_key->r_size) {
        mtbdd_refs_key->r_size *= 2;
        mtbdd_refs_key->results = (MTBDD*)realloc(mtbdd_refs_key->results, sizeof(MTBDD) * mtbdd_refs_key->r_size);
    }
    mtbdd_refs_key->results[mtbdd_refs_key->r_count++] = mtbdd;
    return mtbdd;
}

static inline void
mtbdd_refs_pop(int amount)
{
    LOCALIZE_THREAD_LOCAL(mtbdd_refs_key, mtbdd_refs_internal_t);
    mtbdd_refs_key->r_count-=amount;
}

static inline void
mtbdd_refs_spawn(Task *t)
{
    LOCALIZE_THREAD_LOCAL(mtbdd_refs_key, mtbdd_refs_internal_t);
    if (mtbdd_refs_key->s_count >= mtbdd_refs_key->s_size) {
        mtbdd_refs_key->s_size *= 2;
        mtbdd_refs_key->spawns = (Task**)realloc(mtbdd_refs_key->spawns, sizeof(Task*) * mtbdd_refs_key->s_size);
    }
    mtbdd_refs_key->spawns[mtbdd_refs_key->s_count++] = t;
}

static inline MTBDD
mtbdd_refs_sync(MTBDD result)
{
    LOCALIZE_THREAD_LOCAL(mtbdd_refs_key, mtbdd_refs_internal_t);
    mtbdd_refs_key->s_count--;
    return result;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif