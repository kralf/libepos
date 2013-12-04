/***************************************************************************
 *   Copyright (C) 2008 by Ralf Kaestner                                   *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef EPOS_INTERPOLATED_POSITION_H
#define EPOS_INTERPOLATED_POSITION_H

#include <spline/spline.h>

#include "profile.h"

/** \file interpolated_position.h
  * \brief EPOS interpolated position mode functions
  * 
  * The EPOS interpolated position mode allows for generating trajectories
  * which take the principal form of cubic splines. However, the EPOS-specific
  * spline interpretation sacrifices smoothness by eliminating the requirement
  * for equal second derivatives at the spline knots. Instead, each knot
  * defines a target position and target velocity, and the spline segments
  * can be evaluated based on their two adjacent knots.
  */

/** \name Error Codes
  * \brief Predefined EPOS interpolated position error codes
  */
//@{
#define EPOS_INTERPOLATED_POSITION_ERROR_NONE                  0
//!< Success
#define EPOS_INTERPOLATED_POSITION_ERROR_UNDEFINED             1
//!< Profile undefined at value
//@}

/** \brief Predefined EPOS interpolated position error descriptions
  */
extern const char* epos_interpolated_position_errors[];

/** \name Object Indexes
  * \brief Predefined EPOS interpolated position control object indexes
  */
//@{
#define EPOS_INTERPOLATED_POSITION_INDEX_DATA         0x20C1
//@}

/** \name Control Words
  * \brief Predefined EPOS interpolated position control words
  */
//@{
#define EPOS_INTERPOLATED_POSITION_CONTROL_SET        0x007F
//@}

/** \brief Structure defining a knot of an EPOS interpolated position
  *   control operation
  * 
  * In EPOS terminology, the knots are called PVT (position-velocity-time)
  * reference points.
  */
typedef struct epos_interpolated_position_knot_t {
  double time;               //!< The time of the knot in [s].
  
  float position;            //!< The target position of the knot in [rad].
  float velocity;            //!< The target velocity of the knot in [rad/s].
} epos_interpolated_position_knot_t;

/** \brief Structure defining an EPOS interpolated position control operation
  */
typedef struct epos_interpolated_position_t {
  epos_interpolated_position_knot_t*
    knots;                   //!< The knots of the control operation.
  size_t num_knots;          //!< The number of knots of the control operation.
  
  epos_interpolated_position_knot_t
    start_knot;              //!< The start knot of the profile.
} epos_interpolated_position_t;

/** \brief Initialize EPOS interpolated position control operation
  * \param[in] profile The EPOS interpolated position control operation to be
  *   initialized.
  * \param[in] knots The knots of the EPOS interpolated position profile.
  * \param[in] num_knots The number of knots of the EPOS interpolated position
  *   profile.
  */
void epos_interpolated_position_init(
  epos_interpolated_position_t* profile,
  const epos_interpolated_position_knot_t* knots,
  size_t num_knots);

/** \brief Initialize EPOS interpolated position control operation from a
  *   cubic spline
  * \param[in] profile The EPOS interpolated position control operation to be
  *   initialized.
  * \param[in] spline The spline used to determine the knots of the EPOS
  *   interpolated position profile.
  */
void epos_interpolated_position_init_spline(
  epos_interpolated_position_t* profile,
  const spline_t* spline);

/** \brief Destroy EPOS interpolated position control operation
  * \param[in] profile The EPOS interpolated position control operation to be
  *   destroyed.
  */
void epos_interpolated_position_destroy(
  epos_interpolated_position_t* profile);

/** \brief Start EPOS interpolated position control operation
  * \param[in] node The EPOS node to start the interpolated position control
  *   operation for.
  * \param[in] profile The EPOS interpolated position control operation to be
  *   started.
  * \return The resulting device error code.
  */
int epos_interpolated_position_start(
  epos_node_t* node,
  epos_interpolated_position_t* profile);

/** \brief Stop EPOS interpolated position control operation
  * \param[in] node The EPOS node to stop the interpolated position control
  *   operation for.
  * \return The resulting device error code.
  */
int epos_interpolated_position_stop(
  epos_node_t* node);

/** \brief Find segment of the EPOS interpolated position profile at a
  *   given time
  * \param[in] profile The EPOS interpolated position profile to be
  *   searched for the segment.
  * \param[in] time The time to find the profile segment for in [s].
  * \return The index of the profile segment at the given time or the
  *   negative error code if no such segment exists.
  * 
  * This is a convenience function which searches the entire profile by
  * means of the function epos_interpolated_position_find_segment_bisect().
  */
ssize_t epos_interpolated_position_find_segment(
  const epos_interpolated_position_t* profile,
  double time);

/** \brief Find segment of the EPOS interpolated position profile at a
  *   given time using bisection
  * \param[in] profile The EPOS interpolated position profile to be
  *   searched for the segment.
  * \param[in] time The time to find the profile segment for in [s].
  * \param[in] index_min The lower bound of the search interval defined
  *   over the sequence of segment indexes.
  * \param[in] index_max The upper bound of the search interval defined
  *   over the sequence of segment indexes.
  * \return The index of the profile segment at the given time or the
  *   negative error code if no such segment exists within the search
  *   interval.
  * 
  * Bisection search on the profile is optimal if sequential calls to
  * this function involve random times. The profile will be searched
  * in the interval [index_min, index_max] of segment indexes.
  */
ssize_t epos_interpolated_position_find_segment_bisect(
  const epos_interpolated_position_t* profile,
  double time,
  size_t index_min,
  size_t index_max);

/** \brief Find segment of the EPOS interpolated position profile at a
  *   given time using linear search
  * \param[in] profile The EPOS interpolated position profile to be
  *   searched for the segment.
  * \param[in] time The time to find the profile segment for in [s].
  * \param[in] index_start The segment index at which to start the linear
  *   search.
  * \return The index of the profile segment at the given time or the
  *   negative error code if no such segment exists.
  * 
  * Linear search on the profile is optimal if sequential calls to this
  * function involve incremental or decremental times. The profile will
  * be searched forward or backwards from index_start until a terminal
  * knot is reached.
  */
ssize_t epos_interpolated_position_find_segment_linear(
  const epos_interpolated_position_t* profile,
  double time,
  size_t index_start);

/** \brief Evaluate the values of an EPOS interpolated position profile
  *   segment at a given time
  * \param[in] profile The EPOS interpolated position profile containing the
  *   segment to evaluate the values for.
  * \param[in] index The index of the profile segment to evaluate the values
  *   for.
  * \param[in] time The time to evaluate the profile segment values at in [s].
  * \return The evaluated profile segment values. If no segment exists in
  *   the profile at the specified segment index or the profile segment is
  *   not defined at the given time, all returned profile values will be NaN.
  */
epos_profile_value_t epos_interpolated_position_eval_segment(
  const epos_interpolated_position_t* profile,
  size_t index,
  double time);

/** \brief Evaluate the values of an EPOS interpolated position profile
  *   at a given time
  * \param[in] profile The EPOS interpolated position profile to evaluate
  *   the values for.
  * \param[in] time The time to evaluate the profile values at in [s].
  * \return The evaluated profile values. If the interpolated position
  *   profile is not defined at the given time, all returned profile
  *   values will be NaN.
  * 
  * This is a convenience function, intended to facilitate the computational
  * generation of motion trajectories. It evaluates the profile values by 
  * means of the function epos_interpolated_position_eval_bisect(), allowing
  * for the corresponding segment to be searched on the entire profile.
  */
epos_profile_value_t epos_interpolated_position_eval(
  const epos_interpolated_position_t* profile,
  double time);

/** \brief Evaluate the values of an EPOS interpolated position profile
  *   at a given time using bisection
  * \param[in] profile The EPOS interpolated position profile to evaluate
  *   the values for.
  * \param[in] time The time to evaluate the profile values at in [s].
  * \param[in] index_min The lower bound of the search interval defined
  *   over the sequence of segment indexes.
  * \param[in] index_max The upper bound of the search interval defined
  *   over the sequence of segment indexes.
  * \return The evaluated profile values. If the interpolated position
  *   profile is not defined at the given time and within the provided
  *   interval, all returned profile values will be NaN.
  * 
  * This is a convenience function, intended to facilitate the computational
  * generation of motion trajectories. It calls the function
  * epos_interpolated_position_find_segment_bisect() in order to identify
  * the profile segment at the given time. This is optimal if sequential
  * calls to this function involve random times.
  */
epos_profile_value_t epos_interpolated_position_eval_bisect(
  const epos_interpolated_position_t* profile,
  double time,
  size_t index_min,
  size_t index_max);

/** \brief Evaluate the values of an EPOS interpolated position profile
  *   at a given time using linear search
  * \param[in] profile The EPOS interpolated position profile to evaluate
  *   the values for.
  * \param[in] time The time to evaluate the profile values at in [s].
  * \param[in,out] index The segment index at which to start with the linear
  *   search. On return, the index will be modified to indicate the profile
  *   segment at the given time. If no such segment exists, the returned
  *   index will be undefined.
  * \return The evaluated profile values. If the interpolated position
  *   profile is not defined at the given time, all returned profile values
  *   will be NaN.
  * 
  * This is a convenience function, intended to facilitate the computational
  * generation of motion trajectories. It calls the function
  * epos_interpolated_position_find_segment_linear() in order to identify
  * the profile segment at the given time. This is optimal if sequential
  * calls to this function involve incremental or decremental times.
  */
epos_profile_value_t epos_interpolated_position_eval_linear(
  const epos_interpolated_position_t* profile,
  double time,
  size_t* index);

#endif
