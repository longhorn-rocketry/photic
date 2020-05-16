/**
 * Utilities for working with structs and arrays of structs.
 */

#ifndef PHOTIC_UTIL_STRUCTUTIL_H
#define PHOTIC_UTIL_STRUCTUTIL_H

#include <stdint.h>
#include <vector>

namespace photic
{

/**
 * Binary searches an array of structs for a value embedded within them.
 *
 * @param   k_arr        Array of structs, ordered such that the values of the
 *                       struct member being searched for are ascending.
 * @param   k_ref        Value being searched for.
 * @param   k_ref_ofs    Offset within the struct of the member being searched
 *                       for.
 * @param   k_bound_low  Lower partition index in k_arr.
 * @param   k_bound_high Upper partition index in k_arr.
 * @param   k_res        If an exact match is found, the member struct
 *                       containing the match is stored in here.
 *
 * @ret     Index of match, or where it would go.
 */
template <typename T_ref, typename T_struct>
std::size_t structarr_binsearch(const T_struct* k_arr,
                                const T_ref k_ref,
                                std::size_t k_ref_ofs,
                                std::size_t k_bound_low,
                                std::size_t k_bound_high,
                                T_struct& k_res)

{
    // Compute partition midpoint. If partition is len 0, search is over.
    std::size_t midpoint = k_bound_low + (k_bound_high - k_bound_low) / 2;
    if (k_bound_high - k_bound_low < 1)
    {
        return midpoint;
    }

    // Find the member of interest in the struct at midpoint.
    const uint8_t* arr_alias = (uint8_t*) k_arr;
    T_ref mid_ref = *((T_ref*)(arr_alias + sizeof(T_struct) * midpoint
                               + k_ref_ofs));

    // If that member equals our reference, we're done.
    if (mid_ref == k_ref)
    {
        k_res = ((T_struct*)k_arr)[midpoint];
        return midpoint;
    }
    // If less, repeat on lower half of partition.
    else if (mid_ref < k_ref)
    {
        return structarr_binsearch(k_arr, k_ref, k_ref_ofs, midpoint + 1,
                                   k_bound_high, k_res);
    }
    // If above, repeat on upper half of partition.
    else
    {
        return structarr_binsearch(k_arr, k_ref, k_ref_ofs, k_bound_low,
                                   midpoint, k_res);
    }
}

/**
 * Binary searches an array of structs for a value embedded within them.
 *
 * @param   k_arr        Array of structs, ordered such that the values of the
 *                       struct member being searched for are ascending.
 * @param   k_Len        Length of k_arr.
 * @param   k_ref        Value being searched for.
 * @param   k_ref_ofs    Offset within the struct of the member being searched
 *                       for.
 * @param   k_res        If an exact match is found, the member struct
 *                       containing the match is stored in here.
 *
 * @ret     Index of match, or where it would go.
 */
template <typename T_ref, typename T_struct>
std::size_t structarr_binsearch(const T_struct* k_arr,
                                std::size_t k_len,
                                const T_ref k_ref,
                                std::size_t k_ref_ofs,
                                T_struct& k_res)
{
    return structarr_binsearch(k_arr, k_ref, k_ref_ofs, 0, k_len, k_res);
}

} // namespace photic

#endif
