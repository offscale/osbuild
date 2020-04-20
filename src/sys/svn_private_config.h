/*
 * svn_private_config.hw : Template for svn_private_config.h on Win32.
 *
 * ====================================================================
 *    Licensed to the Apache Software Foundation (ASF) under one
 *    or more contributor license agreements.  See the NOTICE file
 *    distributed with this work for additional information
 *    regarding copyright ownership.  The ASF licenses this file
 *    to you under the Apache License, Version 2.0 (the
 *    "License"); you may not use this file except in compliance
 *    with the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing,
 *    software distributed under the License is distributed on an
 *    "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *    KIND, either express or implied.  See the License for the
 *    specific language governing permissions and limitations
 *    under the License.
 * ====================================================================
 */

/* ==================================================================== */




#ifndef SVN_PRIVATE_CONFIG_HW
#define SVN_PRIVATE_CONFIG_HW


/* Define to a Windows-specific equivalent of config.guess output */
#define SVN_BUILD_HOST "x86-microsoft-windows"

#if defined(_M_X64)
#define SVN_BUILD_TARGET "x64-microsoft-windows"
#elif defined(_M_IA64)
#define SVN_BUILD_TARGET "ia64-microsoft-windows"
#elif defined( _M_IX86)
#define SVN_BUILD_TARGET "x86-microsoft-windows"
#else
#error Unsupported build target.
#endif

/* Path separator for local filesystem */
#define SVN_PATH_LOCAL_SEPARATOR '\\'

#define _(x) (x)
#define Q_(x1, x2, n) (((n) == 1) ? x1 : x2)
#define gettext(x) (x)
#define dgettext(domain, x) (x)
#endif

/* compiler hints as supported by MS VC */
#if defined(SVN_DEBUG)
# define SVN__FORCE_INLINE
# define SVN__PREVENT_INLINE
#elif defined(_MSC_VER)
# define SVN__FORCE_INLINE __forceinline
# define SVN__PREVENT_INLINE __declspec(noinline)
#else
# define SVN__FORCE_INLINE APR_INLINE
# define SVN__PREVENT_INLINE
#endif

#define SVN__PREDICT_TRUE(x)  (x)
#define SVN__PREDICT_FALSE(x)  (x)

/* Macro used to specify that a variable is intentionally left unused.
   Supresses compiler warnings about the variable being unused.  */
#define SVN_UNUSED(v) ( (void)(v) )

#endif /* SVN_PRIVATE_CONFIG_HW */
