#ifndef _XGETOPT_H
#define _XGETOPT_H

/*
  getopt.h - comand line option parsing
  Copyright Keristor Systems and Chris Croughton 1997 - 2005
  Internet: swdev@keristor.org

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/

#if defined(_cplusplus) || defined(__cplusplus)
extern "C" {
#endif

/*
 * This bit of faffing about is because some systems don't like the same
 * functions defined in more than one library, they get confused.  So what
 * we do is to redefine the variables to the names in the interface but
 * fool the linker.  This only makes a difference if you try to debug the
 * getopt code.  Note that we don't have to do it with the structure name
 * or enums, since it's only linkage that's affected.
 */

#define optarg                 _XLIB_optarg
#define optind                 _XLIB_optind
#define opterr                 _XLIB_opterr
#define optopt                 _XLIB_optopt

#define getopt                 _XLIB_getopt
#define getopt_long            _XLIB_getopt_long
#define getopt_long_only       _XLIB_getopt_long_only

extern char *optarg;
extern int   optind;
extern int   opterr;
extern int   optopt;

enum
{
  no_argument,
  required_argument,
  optional_argument
};

struct option
{
  const char *name;
  int         has_arg;
  int        *flag;
  int         val;
};

int getopt(int argc, char * const argv[], const char *optstring);
int getopt_long(int argc, char * const argv[], const char *optstring,
                const struct option *longopts, int *longindex);
int getopt_long_only(int argc, char * const argv[], const char *optstring,
                     const struct option *longopts, int *longindex);

#if defined(_cplusplus) || defined(__cplusplus)
}
#endif

#endif
