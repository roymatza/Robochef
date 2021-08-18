/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 17 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:339  */

/*
Error reporting:
Intention is to provide error token on most bracket expressions,
so synchronisation can occur on next CLOSE_BRAC.
Hence error should be generated for innermost expression containing error.
Expressions which cause errors return a NULL values, and parser
always attempts to carry on.
This won't behave so well if CLOSE_BRAC is missing.

Naming conventions:
Generally, the names should be similar to the PDDL2.1 spec.
During development, they have also been based on older PDDL specs,
older PDDL+ and TIM parsers, and this shows in places.

All the names of fields in the semantic value type begin with t_
Corresponding categories in the grammar begin with c_
Corresponding classes have no prefix.

PDDL grammar       yacc grammar      type of corresponding semantic val.  

thing+             c_things          thing_list
(thing+)           c_thing_list      thing_list

*/

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <ctype.h>

// This is now copied locally to avoid relying on installation 
// of flex++.

//#include "FlexLexer.h"
//#include <FlexLexer.h>

#include "ptree.h"
#include "parse_error.h"

#define YYDEBUG 1 

int yyerror(const char *);

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", ((char *)msgid))
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) ((char *) msgid)
# endif
#endif

extern int yylex();

using namespace VAL;


#line 128 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OPEN_BRAC = 258,
    CLOSE_BRAC = 259,
    OPEN_SQ = 260,
    CLOSE_SQ = 261,
    DEFINE = 262,
    PDDLDOMAIN = 263,
    REQS = 264,
    EQUALITY = 265,
    STRIPS = 266,
    ADL = 267,
    NEGATIVE_PRECONDITIONS = 268,
    TYPING = 269,
    DISJUNCTIVE_PRECONDS = 270,
    EXT_PRECS = 271,
    UNIV_PRECS = 272,
    QUANT_PRECS = 273,
    COND_EFFS = 274,
    FLUENTS = 275,
    TIME = 276,
    DURATIVE_ACTIONS = 277,
    DURATION_INEQUALITIES = 278,
    CONTINUOUS_EFFECTS = 279,
    DERIVED_PREDICATES = 280,
    TIMED_INITIAL_LITERALS = 281,
    PREFERENCES = 282,
    CONSTRAINTS = 283,
    ACTION = 284,
    PROCESS = 285,
    EVENT = 286,
    DURATIVE_ACTION = 287,
    DERIVED = 288,
    CONSTANTS = 289,
    PREDS = 290,
    FUNCTIONS = 291,
    TYPES = 292,
    ARGS = 293,
    PRE = 294,
    CONDITION = 295,
    PREFERENCE = 296,
    START_PRE = 297,
    END_PRE = 298,
    EFFECTS = 299,
    INITIAL_EFFECT = 300,
    FINAL_EFFECT = 301,
    INVARIANT = 302,
    DURATION = 303,
    AT_START = 304,
    AT_END = 305,
    OVER_ALL = 306,
    AND = 307,
    OR = 308,
    EXISTS = 309,
    FORALL = 310,
    IMPLY = 311,
    NOT = 312,
    WHEN = 313,
    WHENEVER = 314,
    EITHER = 315,
    PROBLEM = 316,
    FORDOMAIN = 317,
    INITIALLY = 318,
    OBJECTS = 319,
    GOALS = 320,
    EQ = 321,
    LENGTH = 322,
    SERIAL = 323,
    PARALLEL = 324,
    METRIC = 325,
    MINIMIZE = 326,
    MAXIMIZE = 327,
    HASHT = 328,
    DURATION_VAR = 329,
    TOTAL_TIME = 330,
    INCREASE = 331,
    DECREASE = 332,
    SCALE_UP = 333,
    SCALE_DOWN = 334,
    ASSIGN = 335,
    GREATER = 336,
    GREATEQ = 337,
    LESS = 338,
    LESSEQ = 339,
    Q = 340,
    COLON = 341,
    ALWAYS = 342,
    SOMETIME = 343,
    WITHIN = 344,
    ATMOSTONCE = 345,
    SOMETIMEAFTER = 346,
    SOMETIMEBEFORE = 347,
    ALWAYSWITHIN = 348,
    HOLDDURING = 349,
    HOLDAFTER = 350,
    ISVIOLATED = 351,
    BOGUS = 352,
    NAME = 353,
    FUNCTION_SYMBOL = 354,
    INTVAL = 355,
    FLOATVAL = 356,
    AT_TIME = 357,
    HYPHEN = 358,
    PLUS = 359,
    MUL = 360,
    DIV = 361,
    UMINUS = 362
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 79 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:355  */

    parse_category* t_parse_category;

    effect_lists* t_effect_lists;
    effect* t_effect;
    simple_effect* t_simple_effect;
    cond_effect*   t_cond_effect;
    forall_effect* t_forall_effect;
    timed_effect* t_timed_effect;

    quantifier t_quantifier;
    metric_spec*  t_metric;
    optimization t_optimization;

    symbol* t_symbol;
    var_symbol*   t_var_symbol;
    pddl_type*    t_type;
    pred_symbol*  t_pred_symbol;
    func_symbol*  t_func_symbol;
    const_symbol* t_const_symbol;

    parameter_symbol_list* t_parameter_symbol_list;
    var_symbol_list* t_var_symbol_list;
    const_symbol_list* t_const_symbol_list;
    pddl_type_list* t_type_list;

    proposition* t_proposition;
    pred_decl* t_pred_decl;
    pred_decl_list* t_pred_decl_list;
    func_decl* t_func_decl;
    func_decl_list* t_func_decl_list;

    goal* t_goal;
    con_goal * t_con_goal;
    goal_list* t_goal_list;

    func_term* t_func_term;
    assignment* t_assignment;
    expression* t_expression;
    num_expression* t_num_expression;
    assign_op t_assign_op;
    comparison_op t_comparison_op;

    structure_def* t_structure_def;
    structure_store* t_structure_store;

    action* t_action_def;
    event* t_event_def;
    process* t_process_def;
    durative_action* t_durative_action_def;
    derivation_rule* t_derivation_rule;

    problem* t_problem;
    length_spec* t_length_spec;

    domain* t_domain;    

    pddl_req_flag t_pddl_req_flag;

    plan* t_plan;
    plan_step* t_step;

    int ival;
    double fval;

    char* cp;
    int t_dummy;

    var_symbol_table * vtab;

#line 344 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 361 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  17
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   923

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  108
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  123
/* YYNRULES -- Number of rules.  */
#define YYNRULES  333
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  781

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   362

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   251,   251,   252,   253,   257,   263,   270,   271,   272,
     273,   275,   277,   279,   282,   287,   294,   301,   302,   307,
     309,   314,   316,   324,   332,   334,   342,   347,   349,   353,
     355,   362,   375,   384,   393,   405,   407,   413,   422,   432,
     437,   438,   442,   443,   451,   458,   467,   473,   475,   477,
     484,   490,   494,   498,   502,   507,   514,   519,   521,   525,
     527,   531,   536,   538,   540,   543,   547,   553,   554,   556,
     558,   567,   568,   569,   570,   571,   575,   576,   580,   582,
     584,   591,   592,   593,   595,   599,   601,   609,   611,   619,
     624,   629,   632,   639,   640,   644,   646,   648,   652,   656,
     662,   666,   670,   676,   678,   686,   691,   697,   698,   702,
     703,   707,   709,   711,   718,   719,   720,   722,   727,   729,
     731,   733,   735,   740,   746,   752,   757,   758,   762,   763,
     765,   766,   770,   772,   774,   776,   781,   783,   786,   789,
     795,   796,   797,   805,   809,   812,   816,   821,   828,   833,
     838,   843,   848,   850,   852,   854,   856,   861,   863,   865,
     867,   869,   871,   872,   876,   878,   880,   886,   887,   890,
     893,   895,   913,   915,   917,   923,   924,   925,   926,   927,
     939,   946,   948,   955,   957,   959,   961,   965,   970,   972,
     974,   976,   983,   985,   990,   992,   996,  1001,  1003,  1008,
    1010,  1013,  1015,  1017,  1019,  1021,  1023,  1025,  1027,  1029,
    1031,  1036,  1038,  1042,  1044,  1047,  1050,  1053,  1056,  1062,
    1064,  1069,  1071,  1081,  1088,  1095,  1100,  1105,  1110,  1112,
    1119,  1121,  1128,  1130,  1137,  1139,  1146,  1147,  1151,  1152,
    1153,  1154,  1155,  1159,  1165,  1174,  1185,  1192,  1203,  1209,
    1219,  1225,  1240,  1247,  1249,  1251,  1255,  1257,  1262,  1265,
    1269,  1271,  1273,  1275,  1280,  1285,  1290,  1291,  1293,  1294,
    1296,  1298,  1299,  1300,  1301,  1302,  1304,  1308,  1317,  1320,
    1323,  1325,  1327,  1329,  1331,  1333,  1339,  1343,  1348,  1360,
    1367,  1368,  1369,  1370,  1371,  1373,  1374,  1375,  1378,  1381,
    1384,  1387,  1391,  1393,  1400,  1403,  1407,  1414,  1415,  1420,
    1421,  1422,  1423,  1424,  1426,  1430,  1431,  1432,  1433,  1437,
    1438,  1443,  1444,  1450,  1453,  1455,  1458,  1462,  1466,  1472,
    1476,  1482,  1490,  1491
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OPEN_BRAC", "CLOSE_BRAC", "OPEN_SQ",
  "CLOSE_SQ", "DEFINE", "PDDLDOMAIN", "REQS", "EQUALITY", "STRIPS", "ADL",
  "NEGATIVE_PRECONDITIONS", "TYPING", "DISJUNCTIVE_PRECONDS", "EXT_PRECS",
  "UNIV_PRECS", "QUANT_PRECS", "COND_EFFS", "FLUENTS", "TIME",
  "DURATIVE_ACTIONS", "DURATION_INEQUALITIES", "CONTINUOUS_EFFECTS",
  "DERIVED_PREDICATES", "TIMED_INITIAL_LITERALS", "PREFERENCES",
  "CONSTRAINTS", "ACTION", "PROCESS", "EVENT", "DURATIVE_ACTION",
  "DERIVED", "CONSTANTS", "PREDS", "FUNCTIONS", "TYPES", "ARGS", "PRE",
  "CONDITION", "PREFERENCE", "START_PRE", "END_PRE", "EFFECTS",
  "INITIAL_EFFECT", "FINAL_EFFECT", "INVARIANT", "DURATION", "AT_START",
  "AT_END", "OVER_ALL", "AND", "OR", "EXISTS", "FORALL", "IMPLY", "NOT",
  "WHEN", "WHENEVER", "EITHER", "PROBLEM", "FORDOMAIN", "INITIALLY",
  "OBJECTS", "GOALS", "EQ", "LENGTH", "SERIAL", "PARALLEL", "METRIC",
  "MINIMIZE", "MAXIMIZE", "HASHT", "DURATION_VAR", "TOTAL_TIME",
  "INCREASE", "DECREASE", "SCALE_UP", "SCALE_DOWN", "ASSIGN", "GREATER",
  "GREATEQ", "LESS", "LESSEQ", "Q", "COLON", "ALWAYS", "SOMETIME",
  "WITHIN", "ATMOSTONCE", "SOMETIMEAFTER", "SOMETIMEBEFORE",
  "ALWAYSWITHIN", "HOLDDURING", "HOLDAFTER", "ISVIOLATED", "BOGUS", "NAME",
  "FUNCTION_SYMBOL", "INTVAL", "FLOATVAL", "AT_TIME", "HYPHEN", "PLUS",
  "MUL", "DIV", "UMINUS", "$accept", "mystartsymbol", "c_domain",
  "c_preamble", "c_domain_name", "c_domain_require_def", "c_reqs",
  "c_pred_decls", "c_pred_decl", "c_new_pred_symbol", "c_pred_symbol",
  "c_init_pred_symbol", "c_func_decls", "c_func_decl", "c_new_func_symbol",
  "c_typed_var_list", "c_var_symbol_list", "c_typed_consts",
  "c_const_symbols", "c_new_const_symbols", "c_typed_types",
  "c_parameter_symbols", "c_declaration_var_symbol", "c_var_symbol",
  "c_const_symbol", "c_new_const_symbol", "c_either_type",
  "c_new_primitive_type", "c_primitive_type", "c_new_primitive_types",
  "c_primitive_types", "c_init_els", "c_timed_initial_literal",
  "c_effects", "c_effect", "c_a_effect", "c_p_effect", "c_p_effects",
  "c_conj_effect", "c_da_effect", "c_da_effects", "c_timed_effect",
  "c_cts_only_timed_effect", "c_da_cts_only_effect",
  "c_da_cts_only_effects", "c_a_effect_da", "c_p_effect_da",
  "c_p_effects_da", "c_f_assign_da", "c_proc_effect", "c_proc_effects",
  "c_f_exp_da", "c_binary_expr_da", "c_duration_constraint", "c_d_op",
  "c_d_value", "c_duration_constraints", "c_neg_simple_effect",
  "c_pos_simple_effect", "c_init_neg_simple_effect",
  "c_init_pos_simple_effect", "c_forall_effect", "c_cond_effect",
  "c_assignment", "c_f_exp", "c_f_exp_t", "c_number", "c_f_head",
  "c_ground_f_head", "c_comparison_op", "c_pre_goal_descriptor",
  "c_pref_con_goal", "c_pref_goal", "c_pref_con_goal_list",
  "c_pref_goal_descriptor", "c_constraint_goal_list", "c_constraint_goal",
  "c_goal_descriptor", "c_pre_goal_descriptor_list", "c_goal_list",
  "c_forall", "c_exists", "c_proposition", "c_derived_proposition",
  "c_init_proposition", "c_predicates", "c_functions_def",
  "c_constraints_def", "c_constraints_probdef", "c_structure_defs",
  "c_structure_def", "c_rule_head", "c_derivation_rule", "c_action_def",
  "c_event_def", "c_process_def", "c_durative_action_def", "c_da_def_body",
  "c_da_gd", "c_da_gds", "c_timed_gd", "c_args_head", "c_require_key",
  "c_domain_constants", "c_type_names", "c_problem", "c_problem_body",
  "c_objects", "c_initial_state", "c_goals", "c_goal_spec",
  "c_metric_spec", "c_length_spec", "c_optimization", "c_ground_f_exp",
  "c_binary_ground_f_exp", "c_binary_ground_f_pexps",
  "c_binary_ground_f_mexps", "c_plan", "c_step_t_d", "c_step_d", "c_step",
  "c_float", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362
};
# endif

#define YYPACT_NINF -689

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-689)))

#define YYTABLE_NINF -71

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      41,   111,   -40,  -689,  -689,    57,  -689,  -689,  -689,    70,
    -689,    69,   -19,   217,   125,    70,    70,  -689,   140,  -689,
     -30,   247,    42,   240,  -689,   253,   125,  -689,  -689,    73,
    -689,   156,    23,  -689,   756,   269,   276,   276,   276,   276,
     279,  -689,  -689,  -689,  -689,  -689,  -689,   276,   276,  -689,
    -689,  -689,   283,  -689,   294,   581,   300,    75,    76,    77,
      84,  -689,   213,   386,   357,  -689,   318,  -689,  -689,  -689,
    -689,  -689,   429,  -689,  -689,  -689,  -689,   335,   359,   641,
     384,   657,   387,   393,   319,   428,   319,   434,   319,   436,
     319,  -689,   441,   351,   213,   488,    98,   492,   464,   501,
     231,   535,   -45,     6,   563,   506,  -689,  -689,  -689,  -689,
    -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,
    -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,
     563,   566,  -689,   563,   563,   164,   563,   563,   563,   164,
     164,   164,   568,  -689,  -689,  -689,   569,  -689,   570,  -689,
     572,  -689,   573,  -689,    36,  -689,  -689,   574,  -689,   495,
    -689,  -689,  -689,   105,  -689,  -689,  -689,  -689,    36,  -689,
    -689,  -689,   495,   463,   579,  -689,   479,   582,   303,  -689,
     583,   584,  -689,  -689,   563,   585,   563,   563,   563,   164,
     563,   495,   495,   495,   495,   495,   565,  -689,   213,   213,
    -689,   528,   623,   525,   625,  -689,   495,  -689,  -689,   626,
     563,   563,  -689,   563,   563,   108,  -689,  -689,  -689,  -689,
    -689,    66,   635,   644,  -689,   666,  -689,  -689,  -689,  -689,
    -689,   667,  -689,   668,   669,   563,   563,   670,   671,   672,
     673,   674,   676,  -689,  -689,  -689,  -689,   495,  -689,    36,
    -689,   687,  -689,  -689,  -689,  -689,   321,   329,   563,   688,
     139,   331,  -689,    66,  -689,  -689,   495,   495,   691,  -689,
    -689,  -689,   694,   695,  -689,   566,   661,   662,   664,   658,
     177,  -689,   495,   495,  -689,  -689,  -689,  -689,   704,  -689,
    -689,   607,  -689,  -689,  -689,    66,    66,    66,    66,   706,
     709,   715,   424,   691,   691,   716,   691,   691,   691,   691,
     691,  -689,  -689,   722,   724,   563,   563,   725,  -689,  -689,
    -689,  -689,  -689,  -689,  -689,   168,   186,    45,    66,    66,
      66,  -689,   563,   563,   391,  -689,   213,  -689,   281,   225,
     724,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -689,
     416,   685,  -689,  -689,   686,   689,   190,  -689,  -689,  -689,
    -689,   727,   728,   730,   731,   733,   734,   737,   529,   738,
    -689,   396,   750,   655,   656,   753,  -689,  -689,    49,   755,
      80,   724,   757,   763,   779,   763,   780,   792,  -689,  -689,
    -689,  -689,   711,   304,  -689,  -689,  -689,  -689,  -689,  -689,
    -689,    89,   794,   795,  -689,   257,  -689,  -689,  -689,  -689,
    -689,  -689,   200,   796,  -689,   665,  -689,  -689,  -689,  -689,
     797,  -689,   563,   798,  -689,  -689,   399,   495,   170,   799,
    -689,  -689,  -689,  -689,  -689,  -689,   153,   800,   801,   165,
     165,   403,   732,  -689,   804,   805,   566,   806,  -689,   417,
    -689,   495,   808,    52,  -689,  -689,  -689,  -689,   712,  -689,
     809,   717,  -689,  -689,    49,    49,    49,    49,   810,  -689,
     812,  -689,  -689,  -689,   813,   378,   815,   563,   816,  -689,
    -689,    52,    52,  -689,  -689,   735,   736,  -689,  -689,    66,
     452,  -689,  -689,   346,  -689,  -689,  -689,   818,  -689,  -689,
    -689,   819,   726,   821,   379,   164,   476,   191,   822,  -689,
     823,   206,   214,    49,    49,    49,    49,  -689,  -689,   724,
     824,   638,   825,   827,  -689,  -689,  -689,   827,   827,  -689,
       6,   828,   827,   495,   480,    16,    16,   759,   760,   831,
    -689,    99,   563,   563,   563,  -689,   832,   834,   834,  -689,
     804,   563,    52,    52,    52,    52,    52,   835,  -689,   836,
    -689,   837,  -689,  -689,  -689,  -689,  -689,  -689,  -689,    49,
    -689,    49,  -689,  -689,   838,   733,  -689,  -689,    52,    52,
    -689,  -689,  -689,  -689,  -689,   839,   840,  -689,  -689,   740,
    -689,   842,   843,    66,    66,  -689,   315,   845,   846,   847,
     848,   849,   507,  -689,   645,   850,  -689,  -689,  -689,  -689,
     851,   509,   805,   853,    32,    32,    66,    66,    66,   495,
     594,   854,  -689,  -689,  -689,  -689,   518,    66,    66,  -689,
     763,    53,  -689,  -689,   855,   856,   857,  -689,  -689,  -689,
    -689,  -689,  -689,  -689,    52,    52,    52,    52,    52,  -689,
    -689,  -689,  -689,   858,   250,  -689,   859,   432,   860,   861,
     862,   863,   864,   865,   866,   867,   116,   794,   869,  -689,
     696,  -689,  -689,  -689,  -689,   870,    66,   758,   871,   872,
    -689,   520,    62,    62,    62,    62,    62,  -689,   873,  -689,
     563,    52,    52,   875,  -689,    53,  -689,  -689,  -689,  -689,
    -689,  -689,  -689,   805,   566,   876,   539,   495,  -689,   877,
     878,  -689,  -689,   701,  -689,  -689,  -689,  -689,   459,   766,
     879,  -689,  -689,  -689,   880,   881,   882,   883,  -689,   545,
     853,    16,    16,   495,   112,   884,   885,  -689,  -689,   886,
    -689,  -689,    62,    62,    62,    62,  -689,  -689,  -689,  -689,
    -689,  -689,  -689,  -689,   887,   888,   889,   890,  -689,  -689,
     836,    62,    62,    62,    62,  -689,  -689,  -689,   853,   891,
     892,   893,   894,   895,   896,  -689,  -689,  -689,  -689,  -689,
    -689
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
     326,     0,     0,   333,   332,     0,     2,     3,     4,   326,
     328,   330,     0,     0,    41,   326,   326,     1,     0,   323,
       0,     0,     0,     0,    52,     0,    41,   325,   324,     0,
     327,     0,     0,     6,     0,     0,     0,     0,     0,     0,
      13,   237,   242,   238,   239,   240,   241,     0,     0,   331,
      40,   329,     0,   289,     0,     0,     0,     0,     0,     0,
       0,   243,    43,     0,     0,    58,     0,     5,     7,    10,
      11,    12,     0,   236,     9,     8,    14,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,     0,    39,    43,     0,     0,     0,    20,     0,
       0,     0,    46,     0,     0,     0,    16,    15,   266,   267,
     277,   269,   268,   270,   271,   272,   278,   273,   274,   276,
     275,   279,   280,   281,   282,   283,   284,   285,    17,   233,
       0,     0,   223,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   232,   246,   265,     0,   250,     0,   248,
       0,   252,     0,   286,     0,    42,   229,     0,    23,    36,
     228,    19,   231,     0,   230,    27,   287,    55,     0,    57,
      24,    25,    36,     0,     0,   211,     0,     0,     0,   198,
       0,     0,   167,   168,     0,     0,     0,     0,     0,     0,
       0,    36,    36,    36,    36,    36,     0,    56,    43,    43,
      22,     0,     0,    34,     0,    31,    36,    58,    58,     0,
       0,     0,   224,     0,     0,   179,   175,   176,   177,   178,
      49,     0,     0,     0,   244,     0,   201,   199,   197,   202,
     203,     0,   205,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    60,    38,    37,    50,    36,    21,     0,
      30,     0,    45,    44,   226,   222,     0,     0,     0,     0,
       0,     0,   171,     0,   162,   163,    36,    36,   297,   204,
     206,   207,     0,     0,   210,     0,     0,     0,     0,     0,
       0,    35,    36,    36,    29,   213,   221,   214,     0,   212,
     225,     0,    47,    49,    49,     0,     0,     0,     0,     0,
       0,     0,     0,   297,   297,     0,   297,   297,   297,   297,
     297,   208,   209,     0,     0,     0,     0,     0,    54,    59,
      33,    32,   215,    51,    48,     0,     0,     0,     0,     0,
       0,   218,     0,     0,     0,    65,    43,   300,     0,     0,
       0,   290,   294,   288,   291,   292,   293,   295,   296,   200,
       0,     0,   180,   196,     0,     0,     0,   255,   170,   169,
     157,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     187,     0,     0,     0,     0,     0,   307,   308,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   145,   142,
     141,   140,     0,     0,   159,   158,   160,   161,   216,   217,
     235,     0,     0,     0,   234,     0,   299,    64,    63,    62,
     149,   298,     0,     0,   303,     0,   312,   174,   311,   310,
       0,   301,     0,     0,   220,   196,     0,    36,     0,     0,
      71,    73,    72,    75,    74,   147,     0,     0,     0,     0,
       0,     0,     0,   251,     0,     0,     0,     0,   193,     0,
     187,    36,     0,     0,    26,    65,    49,   305,     0,   306,
       0,     0,    49,    49,     0,     0,     0,     0,     0,   302,
       0,   194,   181,   219,     0,     0,     0,     0,     0,   245,
     127,     0,     0,   249,   247,     0,     0,   136,   144,     0,
       0,   254,   256,     0,   253,    91,    92,     0,   183,   185,
     192,     0,     0,     0,     0,     0,     0,     0,     0,   314,
       0,     0,     0,     0,     0,     0,     0,   309,   195,     0,
       0,     0,     0,    70,    77,    78,    79,    70,    70,    80,
       0,     0,    70,    36,     0,     0,     0,     0,     0,     0,
     143,     0,     0,     0,     0,   259,     0,     0,     0,    94,
       0,     0,     0,     0,     0,     0,     0,     0,   184,     0,
     148,     0,    66,   227,   304,   313,   173,   172,   316,   319,
     315,   321,   317,   318,     0,     0,    86,    84,     0,     0,
      85,    67,    69,    68,   146,     0,     0,   125,   126,     0,
     166,     0,     0,     0,     0,   137,     0,     0,     0,     0,
       0,     0,     0,    99,     0,     0,   110,   113,   111,   112,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    36,
       0,     0,    61,   320,   322,   182,     0,     0,     0,   151,
       0,     0,   123,   124,     0,     0,     0,   264,   260,   261,
     262,   257,   258,   117,     0,     0,     0,     0,     0,    95,
      96,    87,    93,     0,     0,   106,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   186,
       0,    76,    81,    82,    83,     0,     0,     0,     0,     0,
     263,     0,     0,     0,     0,     0,     0,    89,     0,   108,
       0,     0,     0,     0,    90,     0,   153,    97,   154,    98,
     155,   156,   152,     0,     0,     0,     0,    36,   150,     0,
       0,   138,   139,     0,   109,   116,   114,   115,     0,     0,
       0,   128,   130,   131,     0,     0,     0,     0,   102,     0,
       0,     0,     0,    36,     0,     0,     0,   188,   190,     0,
     164,   165,     0,     0,     0,     0,   129,   119,   120,   121,
     122,   118,   103,   107,     0,     0,     0,     0,    88,   189,
       0,     0,     0,     0,     0,   105,   100,   101,     0,     0,
       0,     0,     0,     0,     0,   191,   133,   132,   134,   135,
     104
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -689,  -689,  -689,   427,  -689,   292,  -689,   751,  -689,  -689,
     754,  -689,  -689,  -689,  -689,  -166,   562,  -152,   897,   785,
     325,  -262,  -689,  -689,    40,  -689,    51,  -689,  -155,  -689,
    -689,   418,  -689,   -42,  -378,  -689,  -689,  -689,  -689,  -420,
    -689,  -689,  -689,  -688,  -689,   354,  -689,  -689,   220,   369,
    -689,  -391,  -689,   465,   120,   -43,  -689,  -371,  -340,  -689,
    -689,  -367,  -363,  -443,  -200,  -533,  -130,  -236,  -689,  -689,
    -317,  -326,   144,   238,  -689,  -689,   -56,  -100,  -689,   697,
     -80,  -689,  -368,  -689,   455,  -689,  -689,  -689,  -689,  -689,
     874,  -689,  -689,  -689,  -689,  -689,  -689,  -689,  -482,  -689,
    -320,   288,  -689,  -689,  -689,  -689,   333,  -689,  -689,  -689,
    -689,  -689,  -689,  -689,  -327,  -689,   340,   339,   353,  -689,
     898,  -689,   900
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,    35,    23,   303,    79,    97,    98,   159,
     220,   456,   100,   165,   206,   202,   203,    92,    25,    93,
     101,   260,   247,   324,   292,    94,   198,   169,   199,   102,
     280,   371,   407,   522,   429,   523,   524,   626,   430,   494,
     611,   495,   655,   656,   729,   605,   606,   681,   607,   437,
     534,   720,   721,   357,   392,   539,   441,   525,   526,   408,
     409,   527,   528,   529,   540,   591,   264,   265,   419,   221,
     351,   448,   621,   449,   352,   178,   313,   353,   426,   256,
     478,   223,   175,   104,   410,    37,    38,    39,   304,    40,
      41,    66,    42,    43,    44,    45,    46,   393,   491,   602,
     492,   146,   128,    47,    48,     7,   305,   306,   307,   340,
     308,   309,   310,   378,   569,   468,   570,   572,     8,     9,
      10,    11,    12
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      82,   142,   496,   592,   174,   184,   209,   438,   369,   188,
     189,   190,   431,   208,   431,   435,   433,   435,   433,   589,
     434,   263,   434,   379,    53,   238,   239,   240,   241,   242,
     177,   325,   326,   180,   181,   657,   185,   186,   187,   196,
     251,   753,   754,   432,     1,   432,   244,   245,   261,   360,
      31,   420,   415,   167,    26,   504,   261,    17,   168,   236,
      15,    16,     2,   299,   424,   718,    26,    21,   612,   261,
       3,     4,   170,    18,    20,   179,    83,    85,    87,    51,
     774,   659,   661,   173,   231,    89,   233,   234,   235,   590,
     237,     2,    81,   222,   283,   327,   328,   329,   330,   157,
     300,   301,   596,    32,   171,   590,   204,   435,   531,   473,
     255,   255,   -24,   258,   259,   261,   320,   321,    13,    81,
     642,    54,   228,   500,   416,   319,   676,   361,   362,   363,
     364,   262,   182,   183,   197,   272,   273,   513,   514,   515,
     516,     3,     4,   290,   262,   182,   183,   719,   417,   182,
     183,   262,   262,   182,   183,   435,   286,   286,   288,   435,
     435,   262,   182,   183,   435,   262,   182,   183,   496,   496,
       3,     4,   358,    84,    86,    88,   608,   608,   422,   435,
     435,   318,    90,   674,   372,   710,   568,   446,   571,   573,
     359,   652,   653,   -24,   507,   563,   158,   597,   755,   756,
     511,   512,   574,   205,   457,   480,   -24,   609,   609,    14,
     566,   262,   182,   183,   704,   354,   355,   505,   567,   207,
      22,   598,   475,    24,   291,   132,   375,   476,   477,   481,
     482,   389,   365,   366,   163,   164,   170,    24,    14,   386,
     387,    33,   388,    34,   571,   535,   536,   390,   418,   391,
      18,   688,   675,   291,    52,   672,   389,    49,   435,   431,
     496,   474,   435,   433,   182,   183,    24,   434,   171,   458,
     382,   291,   390,    67,   391,   197,   291,   636,   370,    34,
     423,   425,    72,   735,    24,   501,   673,    76,   403,    24,
     432,   291,   724,   725,   726,   727,   376,   377,    77,   291,
     282,    80,   689,    81,    24,   132,    81,   227,   443,   690,
     716,    91,    24,   435,   452,    36,   614,   615,   616,   617,
     618,   103,   470,   453,   173,   285,   691,   692,    36,    36,
      36,    36,   173,   287,   418,   418,   418,   418,   105,    36,
      36,   717,   627,   628,   444,   447,   450,   546,   445,   373,
     374,   761,   762,   763,   764,   454,   541,   145,    99,   455,
     -28,   -28,    19,   106,   542,   543,   544,   586,    27,    28,
     770,   771,   772,   773,   148,   561,   150,   532,   152,   520,
     500,   521,   -70,   418,   418,   418,   418,    95,   129,    96,
     497,   143,   367,   370,   368,   547,   548,   144,   549,   405,
     406,   132,   350,   472,   550,   551,   356,   487,   682,   683,
     684,   685,   686,   557,   658,   660,   662,   663,   664,   575,
     368,   499,   552,   553,   554,   555,   556,   658,   660,   293,
     294,   677,   147,    55,   295,   296,   297,   298,   149,   418,
     151,   418,   599,   600,   601,   153,   723,   723,   723,   723,
     723,   613,   334,   665,   154,   731,   732,   380,    57,    58,
      59,    60,    61,    68,    69,    70,    71,    96,   381,   211,
     212,   132,   213,   214,    74,    75,   709,   293,   294,   405,
     562,   581,   215,   436,   587,   582,   583,   335,   336,   337,
     585,   338,   156,   541,   339,   734,   160,   216,   217,   218,
     219,   542,   543,   544,   545,   162,   723,   723,   723,   723,
     490,   641,   493,   651,   171,   210,   211,   212,   132,   213,
     214,   670,   671,   713,   714,   723,   723,   723,   723,   215,
     293,   294,   252,   253,   363,   295,   296,   695,   298,   166,
     668,   739,   368,   738,   216,   217,   218,   219,   654,   752,
     634,   635,   722,   722,   722,   722,   722,   293,   294,   485,
     486,   171,   742,   743,   744,   745,   173,   757,   176,    81,
     401,   191,   192,   193,   693,   194,   195,   225,   200,   130,
     201,   402,    78,   224,   132,   -18,   226,   229,   230,   232,
     730,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,
     -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,   -18,
     705,   450,   722,   722,   722,   722,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   243,   246,   248,   249,   250,
     254,   722,   722,   722,   722,   666,   341,   342,   266,   344,
     345,   346,   347,   348,   130,   107,   667,   267,   736,   132,
     370,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     268,   269,   270,   271,   274,   275,   276,   277,   278,   -18,
     279,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     577,   284,   289,   132,   302,   476,   477,   643,   311,   312,
     314,   315,   476,   316,   170,   323,   317,   130,   322,   131,
     331,   170,   132,   332,   578,   579,   554,   555,   556,   333,
     343,   644,   645,   646,   647,   648,   349,   350,   356,   383,
     384,   394,   395,   385,   396,   397,   171,   398,   399,   127,
     460,   400,   404,   171,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   476,   411,   412,   413,   414,   476,   421,
     427,   461,   170,   462,   463,    55,   428,   170,   464,   465,
     466,   467,   578,   579,   554,   555,   556,   644,   645,   646,
     647,   648,   436,   439,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,   171,   440,   442,   368,   451,   171,
     459,   469,   471,   479,   483,   484,   489,   490,   493,   281,
     498,   502,   508,   509,   517,   510,   518,   519,   530,   533,
     537,   538,   558,   559,   454,   560,   564,   565,   576,   580,
     521,   710,   584,   593,   594,   595,   603,   604,   619,   620,
     746,   622,   625,   629,   630,   631,   632,   633,   596,   161,
     637,   638,   639,   640,   649,   650,   654,   172,   669,   678,
     679,   680,   687,   694,   696,   697,   698,   699,   700,   701,
     702,   703,   707,   506,   708,   711,   712,   728,   733,   155,
     737,   740,   741,   747,   748,   749,   750,   751,   758,   759,
     760,   765,   766,   767,   768,   775,   776,   777,   778,   779,
     780,   715,   610,   588,   769,   706,   488,   503,   257,   623,
     624,     0,     0,     0,    73,     0,     0,     0,     0,    30,
      29,     0,     0,    50
};

static const yytype_int16 yycheck[] =
{
      56,    81,   445,   536,   104,   135,   172,   385,   334,   139,
     140,   141,   383,   168,   385,   383,   383,   385,   385,     3,
     383,   221,   385,   340,     1,   191,   192,   193,   194,   195,
     130,   293,   294,   133,   134,     3,   136,   137,   138,     3,
     206,   729,   730,   383,     3,   385,   198,   199,     3,     4,
       8,   378,     3,    98,    14,     3,     3,     0,   103,   189,
     100,   101,    21,   263,   381,     3,    26,    86,   550,     3,
     100,   101,    66,     3,     5,   131,     1,     1,     1,     6,
     768,   614,   615,     3,   184,     1,   186,   187,   188,    73,
     190,    21,     3,   173,   249,   295,   296,   297,   298,     1,
     266,   267,     3,    61,    98,    73,     1,   475,   476,   426,
     210,   211,     4,   213,   214,     3,   282,   283,     7,     3,
     602,    98,   178,   449,    75,   280,    73,   327,   328,   329,
     330,    99,   100,   101,    98,   235,   236,   464,   465,   466,
     467,   100,   101,     4,    99,   100,   101,    85,    99,   100,
     101,    99,    99,   100,   101,   523,   256,   257,   258,   527,
     528,    99,   100,   101,   532,    99,   100,   101,   611,   612,
     100,   101,     4,    98,    98,    98,   547,   548,    98,   547,
     548,     4,    98,   626,   336,    73,   513,    98,   515,   516,
       4,   611,   612,    85,   456,     4,    98,    98,   731,   732,
     462,   463,   519,    98,     4,    52,    98,   547,   548,    98,
       4,    99,   100,   101,    98,   315,   316,   453,     4,   168,
       3,   541,    52,    98,    85,    55,     1,    57,    58,    76,
      77,    66,   332,   333,     3,     4,    66,    98,    98,    49,
      50,     1,    52,     3,   571,   481,   482,    82,   378,    84,
       3,     1,   630,    85,    98,   626,    66,     4,   626,   630,
     703,   427,   630,   630,   100,   101,    98,   630,    98,    69,
     350,    85,    82,     4,    84,    98,    85,   597,   334,     3,
     380,   381,     3,   703,    98,   451,   626,     4,   368,    98,
     630,    85,   683,   684,   685,   686,    71,    72,     4,    85,
     249,     1,    52,     3,    98,    55,     3,     4,     4,    59,
     681,    98,    98,   681,    57,    23,   552,   553,   554,   555,
     556,     3,   422,    66,     3,     4,    76,    77,    36,    37,
      38,    39,     3,     4,   464,   465,   466,   467,     3,    47,
      48,   681,   578,   579,    40,   401,   402,     1,    44,    68,
      69,   742,   743,   744,   745,    98,    41,    38,     1,   102,
       3,     4,     9,     4,    49,    50,    51,   533,    15,    16,
     761,   762,   763,   764,    86,   505,    88,   477,    90,     1,
     706,     3,     4,   513,   514,   515,   516,     1,     4,     3,
     446,     4,     1,   449,     3,    49,    50,     4,    52,     3,
       4,    55,     3,     4,    58,    59,     3,     4,   644,   645,
     646,   647,   648,   493,   614,   615,   616,   617,   618,   519,
       3,     4,    76,    77,    78,    79,    80,   627,   628,    98,
      99,   631,     4,     9,   103,   104,   105,   106,     4,   569,
       4,   571,   542,   543,   544,     4,   682,   683,   684,   685,
     686,   551,    28,   619,   103,   691,   692,    41,    29,    30,
      31,    32,    33,    36,    37,    38,    39,     3,    52,    53,
      54,    55,    56,    57,    47,    48,   676,    98,    99,     3,
       4,   523,    66,     3,     4,   527,   528,    63,    64,    65,
     532,    67,     4,    41,    70,   695,     4,    81,    82,    83,
      84,    49,    50,    51,    52,     4,   742,   743,   744,   745,
       3,     4,     3,     4,    98,    52,    53,    54,    55,    56,
      57,     3,     4,     3,     4,   761,   762,   763,   764,    66,
      98,    99,   207,   208,   734,   103,   104,   105,   106,     4,
     620,   707,     3,     4,    81,    82,    83,    84,     3,     4,
     593,   594,   682,   683,   684,   685,   686,    98,    99,   439,
     440,    98,   103,   104,   105,   106,     3,   733,    62,     3,
      41,     3,     3,     3,   654,     3,     3,    98,     4,    50,
      85,    52,     1,     4,    55,     4,     4,     4,     4,     4,
     690,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
     666,   667,   742,   743,   744,   745,    87,    88,    89,    90,
      91,    92,    93,    94,    95,    60,    98,     4,   103,     4,
       4,   761,   762,   763,   764,    41,   303,   304,     3,   306,
     307,   308,   309,   310,    50,     4,    52,     3,   704,    55,
     706,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
       4,     4,     4,     4,     4,     4,     4,     4,     4,    98,
       4,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      52,     4,     4,    55,     3,    57,    58,    52,     4,     4,
      39,    39,    57,    39,    66,    98,    48,    50,     4,    52,
       4,    66,    55,     4,    76,    77,    78,    79,    80,     4,
       4,    76,    77,    78,    79,    80,     4,     3,     3,    44,
      44,     4,     4,    44,     4,     4,    98,     4,     4,    98,
      75,     4,     4,    98,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    57,     4,   100,   100,     4,    57,     4,
       3,    96,    66,    98,    99,     9,     3,    66,   103,   104,
     105,   106,    76,    77,    78,    79,    80,    76,    77,    78,
      79,    80,     3,     3,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    98,     3,    85,     3,     3,    98,
       4,     4,     4,     4,     4,     4,    74,     3,     3,   247,
       4,     3,   100,     4,     4,    98,     4,     4,     3,     3,
      85,    85,     4,     4,    98,     4,     4,     4,     4,     4,
       3,    73,     4,    74,    74,     4,     4,     3,     3,     3,
      74,     4,     4,     4,     4,   105,     4,     4,     3,    98,
       4,     4,     4,     4,     4,     4,     3,   103,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     3,   455,     4,     4,     4,     4,     3,    94,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,   681,   548,   534,   760,   667,   441,   452,   211,   569,
     571,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,    21,
      20,    -1,    -1,    26
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    21,   100,   101,   109,   110,   213,   226,   227,
     228,   229,   230,     7,    98,   100,   101,     0,     3,   226,
       5,    86,     3,   112,    98,   126,   132,   226,   226,   230,
     228,     8,    61,     1,     3,   111,   113,   193,   194,   195,
     197,   198,   200,   201,   202,   203,   204,   211,   212,     4,
     126,     6,    98,     1,    98,     9,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,   199,     4,   111,   111,
     111,   111,     3,   198,   111,   111,     4,     4,     1,   114,
       1,     3,   184,     1,    98,     1,    98,     1,    98,     1,
      98,    98,   125,   127,   133,     1,     3,   115,   116,     1,
     120,   128,   137,     3,   191,     3,     4,     4,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    98,   210,     4,
      50,    52,    55,    87,    88,    89,    90,    91,    92,    93,
      94,    95,   188,     4,     4,    38,   209,     4,   209,     4,
     209,     4,   209,     4,   103,   127,     4,     1,    98,   117,
       4,   115,     4,     3,     4,   121,     4,    98,   103,   135,
      66,    98,   118,     3,   185,   190,    62,   185,   183,   184,
     185,   185,   100,   101,   174,   185,   185,   185,   174,   174,
     174,     3,     3,     3,     3,     3,     3,    98,   134,   136,
       4,    85,   123,   124,     1,    98,   122,   134,   136,   123,
      52,    53,    54,    56,    57,    66,    81,    82,    83,    84,
     118,   177,   188,   189,     4,    98,     4,     4,   184,     4,
       4,   185,     4,   185,   185,   185,   174,   185,   123,   123,
     123,   123,   123,    60,   125,   125,    98,   130,     4,   103,
       4,   123,   128,   128,     4,   185,   187,   187,   185,   185,
     129,     3,    99,   172,   174,   175,     3,     3,     4,     4,
       4,     4,   185,   185,     4,     4,     4,     4,     4,     4,
     138,   124,   134,   136,     4,     4,   185,     4,   185,     4,
       4,    85,   132,    98,    99,   103,   104,   105,   106,   172,
     123,   123,     3,   113,   196,   214,   215,   216,   218,   219,
     220,     4,     4,   184,    39,    39,    39,    48,     4,   136,
     123,   123,     4,    98,   131,   129,   129,   172,   172,   172,
     172,     4,     4,     4,    28,    63,    64,    65,    67,    70,
     217,   214,   214,     4,   214,   214,   214,   214,   214,     4,
       3,   178,   182,   185,   185,   185,     3,   161,     4,     4,
       4,   172,   172,   172,   172,   185,   185,     1,     3,   179,
     184,   139,   125,    68,    69,     1,    71,    72,   221,   178,
      41,    52,   188,    44,    44,    44,    49,    50,    52,    66,
      82,    84,   162,   205,     4,     4,     4,     4,     4,     4,
       4,    41,    52,   188,     4,     3,     4,   140,   167,   168,
     192,     4,   100,   100,     4,     3,    75,    99,   174,   176,
     222,     4,    98,   185,   178,   185,   186,     3,     3,   142,
     146,   165,   166,   169,   170,   190,     3,   157,   142,     3,
       3,   164,    85,     4,    40,    44,    98,   184,   179,   181,
     184,     3,    57,    66,    98,   102,   119,     4,    69,     4,
      75,    96,    98,    99,   103,   104,   105,   106,   223,     4,
     185,     4,     4,   178,   123,    52,    57,    58,   188,     4,
      52,    76,    77,     4,     4,   162,   162,     4,   161,    74,
       3,   206,   208,     3,   147,   149,   171,   184,     4,     4,
     179,   123,     3,   192,     3,   175,   139,   129,   100,     4,
      98,   129,   129,   222,   222,   222,   222,     4,     4,     4,
       1,     3,   141,   143,   144,   165,   166,   169,   170,   171,
       3,   190,   185,     3,   158,   175,   175,    85,    85,   163,
     172,    41,    49,    50,    51,    52,     1,    49,    50,    52,
      58,    59,    76,    77,    78,    79,    80,   188,     4,     4,
       4,   174,     4,     4,     4,     4,     4,     4,   222,   222,
     224,   222,   225,   222,   178,   185,     4,    52,    76,    77,
       4,   141,   141,   141,     4,   141,   123,     4,   157,     3,
      73,   173,   173,    74,    74,     4,     3,    98,   208,   185,
     185,   185,   207,     4,     3,   153,   154,   156,   165,   166,
     153,   148,   206,   185,   175,   175,   175,   175,   175,     3,
       3,   180,     4,   224,   225,     4,   145,   175,   175,     4,
       4,   105,     4,     4,   163,   163,   208,     4,     4,     4,
       4,     4,   206,    52,    76,    77,    78,    79,    80,     4,
       4,     4,   147,   147,     3,   150,   151,     3,   172,   173,
     172,   173,   172,   172,   172,   123,    41,    52,   188,     4,
       3,     4,   165,   166,   171,   142,    73,   172,     4,     4,
       4,   155,   175,   175,   175,   175,   175,     4,     1,    52,
      59,    76,    77,   188,     4,   105,     4,     4,     4,     4,
       4,     4,     4,     4,    98,   184,   181,     3,     4,   172,
      73,     4,     4,     3,     4,   156,   165,   166,     3,    85,
     159,   160,   174,   175,   159,   159,   159,   159,     4,   152,
     185,   175,   175,     3,   172,   147,   184,     4,     4,   123,
       4,     4,   103,   104,   105,   106,    74,     4,     4,     4,
       4,     4,     4,   151,   151,   173,   173,   123,     4,     4,
       4,   159,   159,   159,   159,     4,     4,     4,     4,   180,
     159,   159,   159,   159,   151,     4,     4,     4,     4,     4,
       4
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   108,   109,   109,   109,   110,   110,   111,   111,   111,
     111,   111,   111,   111,   112,   113,   113,   114,   114,   115,
     115,   116,   116,   117,   118,   118,   119,   120,   120,   121,
     121,   122,   123,   123,   123,   124,   124,   125,   125,   125,
     126,   126,   127,   127,   128,   128,   128,   129,   129,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   137,   138,
     138,   139,   139,   139,   139,   139,   140,   141,   141,   141,
     141,   142,   142,   142,   142,   142,   143,   143,   144,   144,
     144,   145,   145,   145,   145,   146,   146,   147,   147,   147,
     147,   147,   147,   148,   148,   149,   149,   149,   149,   149,
     150,   150,   150,   151,   151,   151,   151,   152,   152,   153,
     153,   154,   154,   154,   155,   155,   155,   155,   156,   156,
     156,   156,   156,   157,   157,   157,   158,   158,   159,   159,
     159,   159,   160,   160,   160,   160,   161,   161,   161,   161,
     162,   162,   162,   163,   164,   164,   165,   166,   167,   168,
     169,   170,   171,   171,   171,   171,   171,   172,   172,   172,
     172,   172,   172,   172,   173,   173,   173,   174,   174,   175,
     175,   175,   176,   176,   176,   177,   177,   177,   177,   177,
     178,   178,   178,   179,   179,   179,   179,   179,   180,   180,
     180,   180,   181,   181,   182,   182,   182,   183,   183,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   185,   185,   185,   185,   185,   185,   185,   185,   186,
     186,   187,   187,   188,   189,   190,   191,   192,   193,   193,
     194,   194,   195,   195,   196,   196,   197,   197,   198,   198,
     198,   198,   198,   199,   200,   201,   201,   202,   202,   203,
     203,   204,   204,   205,   205,   205,   206,   206,   207,   207,
     208,   208,   208,   208,   208,   209,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   210,   210,   210,   210,
     210,   210,   210,   210,   210,   210,   211,   212,   213,   213,
     214,   214,   214,   214,   214,   214,   214,   214,   215,   216,
     217,   218,   219,   219,   220,   220,   220,   221,   221,   222,
     222,   222,   222,   222,   222,   223,   223,   223,   223,   224,
     224,   225,   225,   226,   226,   226,   226,   227,   227,   228,
     228,   229,   230,   230
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     5,     4,     2,     2,     2,
       2,     2,     2,     1,     4,     4,     4,     2,     0,     2,
       1,     4,     3,     1,     1,     1,     1,     2,     0,     4,
       3,     1,     4,     4,     1,     3,     0,     4,     4,     1,
       2,     0,     2,     0,     4,     4,     1,     2,     3,     0,
       1,     1,     1,     1,     4,     1,     1,     2,     0,     2,
       0,     6,     2,     2,     2,     0,     4,     2,     2,     2,
       0,     1,     1,     1,     1,     1,     4,     1,     1,     1,
       1,     2,     2,     2,     0,     4,     4,     4,     7,     5,
       5,     1,     1,     2,     0,     4,     4,     5,     5,     3,
       5,     5,     3,     4,     7,     5,     1,     2,     0,     4,
       1,     1,     1,     1,     2,     2,     2,     0,     5,     5,
       5,     5,     5,     5,     5,     4,     2,     0,     1,     2,
       1,     1,     5,     5,     5,     5,     4,     6,     9,     9,
       1,     1,     1,     1,     2,     0,     4,     1,     4,     1,
       7,     5,     5,     5,     5,     5,     5,     4,     5,     5,
       5,     5,     1,     1,     5,     5,     1,     1,     1,     4,
       4,     1,     4,     4,     1,     1,     1,     1,     1,     1,
       1,     4,     7,     4,     5,     4,     7,     1,     4,     5,
       4,     7,     2,     1,     4,     5,     1,     2,     1,     4,
       7,     4,     4,     4,     5,     4,     5,     5,     6,     6,
       5,     1,     4,     4,     4,     5,     7,     7,     5,     2,
       1,     2,     1,     1,     1,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     2,     1,     1,     1,
       1,     1,     1,     1,     5,    12,     4,    12,     4,    12,
       4,    11,     4,     3,     3,     0,     1,     4,     2,     0,
       4,     4,     4,     5,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     4,    12,     5,
       2,     2,     2,     2,     2,     2,     2,     0,     4,     4,
       1,     4,     5,     4,     7,     5,     5,     1,     1,     3,
       1,     1,     1,     4,     3,     3,     3,     3,     3,     1,
       2,     1,     2,     2,     3,     3,     0,     3,     1,     4,
       1,     4,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 251 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {top_thing= (yyvsp[0].t_domain); current_analysis->the_domain= (yyvsp[0].t_domain);}
#line 2041 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 3:
#line 252 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {top_thing= (yyvsp[0].t_problem); current_analysis->the_problem= (yyvsp[0].t_problem);}
#line 2047 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 4:
#line 253 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {top_thing= (yyvsp[0].t_plan); }
#line 2053 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 5:
#line 258 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[-1].t_domain); (yyval.t_domain)->name= (yyvsp[-2].cp);delete [] (yyvsp[-2].cp);
	if (types_used && !types_defined) {
		yyerrok; log_error(E_FATAL,"Syntax error in domain - no :types section, but types used in definitions."); 
	}
	}
#line 2063 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 6:
#line 264 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_domain)=static_cast<domain*>(NULL);
       	log_error(E_FATAL,"Syntax error in domain"); }
#line 2070 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 7:
#line 270 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain); (yyval.t_domain)->req= (yyvsp[-1].t_pddl_req_flag);}
#line 2076 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 8:
#line 271 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {types_defined = true; (yyval.t_domain)= (yyvsp[0].t_domain); (yyval.t_domain)->types= (yyvsp[-1].t_type_list);}
#line 2082 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 9:
#line 272 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain); (yyval.t_domain)->constants= (yyvsp[-1].t_const_symbol_list);}
#line 2088 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 10:
#line 273 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain); 
                                       (yyval.t_domain)->predicates= (yyvsp[-1].t_pred_decl_list); }
#line 2095 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 11:
#line 275 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain); 
                                       (yyval.t_domain)->functions= (yyvsp[-1].t_func_decl_list); }
#line 2102 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 12:
#line 277 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_domain)= (yyvsp[0].t_domain);
   										(yyval.t_domain)->constraints = (yyvsp[-1].t_con_goal);}
#line 2109 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 13:
#line 279 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_domain)= new domain((yyvsp[0].t_structure_store)); }
#line 2115 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 14:
#line 282 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.cp)=(yyvsp[-1].cp);}
#line 2121 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 15:
#line 288 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {
	// Stash in analysis object --- we need to refer to it during parse
	//   but domain object is not created yet,
	current_analysis->req |= (yyvsp[-1].t_pddl_req_flag);
	(yyval.t_pddl_req_flag)=(yyvsp[-1].t_pddl_req_flag);
    }
#line 2132 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 16:
#line 295 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
       log_error(E_FATAL,"Syntax error in requirements declaration.");
       (yyval.t_pddl_req_flag)= 0; }
#line 2140 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 17:
#line 301 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_pddl_req_flag)= (yyvsp[-1].t_pddl_req_flag) | (yyvsp[0].t_pddl_req_flag); }
#line 2146 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 18:
#line 302 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_pddl_req_flag)= 0; }
#line 2152 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 19:
#line 308 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pred_decl_list)=(yyvsp[0].t_pred_decl_list); (yyval.t_pred_decl_list)->push_front((yyvsp[-1].t_pred_decl));}
#line 2158 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 20:
#line 310 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {  (yyval.t_pred_decl_list)=new pred_decl_list;
           (yyval.t_pred_decl_list)->push_front((yyvsp[0].t_pred_decl)); }
#line 2165 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 21:
#line 315 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pred_decl)= new pred_decl((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_var_symbol_list),current_analysis->var_tab_stack.pop());}
#line 2171 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 22:
#line 317 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
        // hope someone makes this error someday
        log_error(E_FATAL,"Syntax error in predicate declaration.");
	(yyval.t_pred_decl)= NULL; }
#line 2180 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 23:
#line 325 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_put((yyvsp[0].cp));
           current_analysis->var_tab_stack.push(
           				current_analysis->buildPredTab());
           delete [] (yyvsp[0].cp); }
#line 2189 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 24:
#line 332 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_ref("="); 
	      requires(E_EQUALITY); }
#line 2196 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 25:
#line 334 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2202 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 26:
#line 342 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_pred_symbol)=current_analysis->pred_tab.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2208 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 27:
#line 348 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_func_decl_list)=(yyvsp[-1].t_func_decl_list); (yyval.t_func_decl_list)->push_back((yyvsp[0].t_func_decl));}
#line 2214 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 28:
#line 349 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_decl_list)=new func_decl_list; }
#line 2220 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 29:
#line 354 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_func_decl)= new func_decl((yyvsp[-2].t_func_symbol),(yyvsp[-1].t_var_symbol_list),current_analysis->var_tab_stack.pop());}
#line 2226 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 30:
#line 356 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
	 log_error(E_FATAL,"Syntax error in functor declaration.");
	 (yyval.t_func_decl)= NULL; }
#line 2234 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 31:
#line 363 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_symbol)=current_analysis->func_tab.symbol_put((yyvsp[0].cp));
           current_analysis->var_tab_stack.push(
           		current_analysis->buildFuncTab()); 
           delete [] (yyvsp[0].cp); }
#line 2243 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 32:
#line 376 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {  
      (yyval.t_var_symbol_list)= (yyvsp[-3].t_var_symbol_list);
      (yyval.t_var_symbol_list)->set_types((yyvsp[-1].t_type));           /* Set types for variables */
      (yyval.t_var_symbol_list)->splice((yyval.t_var_symbol_list)->end(),*(yyvsp[0].t_var_symbol_list));   /* Join lists */ 
      delete (yyvsp[0].t_var_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   }
#line 2256 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 33:
#line 385 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {  
      (yyval.t_var_symbol_list)= (yyvsp[-3].t_var_symbol_list);
      (yyval.t_var_symbol_list)->set_either_types((yyvsp[-1].t_type_list));    /* Set types for variables */
      (yyval.t_var_symbol_list)->splice((yyval.t_var_symbol_list)->end(),*(yyvsp[0].t_var_symbol_list));   /* Join lists */ 
      delete (yyvsp[0].t_var_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   }
#line 2269 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 34:
#line 394 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {
       (yyval.t_var_symbol_list)= (yyvsp[0].t_var_symbol_list);
   }
#line 2277 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 35:
#line 406 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_var_symbol_list)=(yyvsp[0].t_var_symbol_list); (yyvsp[0].t_var_symbol_list)->push_front((yyvsp[-1].t_var_symbol)); }
#line 2283 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 36:
#line 407 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_var_symbol_list)= new var_symbol_list; }
#line 2289 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 37:
#line 414 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {  
      (yyval.t_const_symbol_list)= (yyvsp[-3].t_const_symbol_list);
      (yyvsp[-3].t_const_symbol_list)->set_types((yyvsp[-1].t_type));           /* Set types for constants */
      (yyvsp[-3].t_const_symbol_list)->splice((yyvsp[-3].t_const_symbol_list)->end(),*(yyvsp[0].t_const_symbol_list)); /* Join lists */ 
      delete (yyvsp[0].t_const_symbol_list);                   /* Delete (now empty) list */
      requires(E_TYPING);
      types_used = true;
   }
#line 2302 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 38:
#line 423 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {  
      (yyval.t_const_symbol_list)= (yyvsp[-3].t_const_symbol_list);
      (yyvsp[-3].t_const_symbol_list)->set_either_types((yyvsp[-1].t_type_list));
      (yyvsp[-3].t_const_symbol_list)->splice((yyvsp[-3].t_const_symbol_list)->end(),*(yyvsp[0].t_const_symbol_list));
      delete (yyvsp[0].t_const_symbol_list);
      requires(E_TYPING);
      types_used = true;
   }
#line 2315 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 39:
#line 432 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)= (yyvsp[0].t_const_symbol_list);}
#line 2321 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 40:
#line 437 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[0].t_const_symbol_list); (yyvsp[0].t_const_symbol_list)->push_front((yyvsp[-1].t_const_symbol));}
#line 2327 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 41:
#line 438 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=new const_symbol_list;}
#line 2333 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 42:
#line 442 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[0].t_const_symbol_list); (yyvsp[0].t_const_symbol_list)->push_front((yyvsp[-1].t_const_symbol));}
#line 2339 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 43:
#line 443 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=new const_symbol_list;}
#line 2345 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 44:
#line 452 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {  
       (yyval.t_type_list)= (yyvsp[-3].t_type_list);
       (yyval.t_type_list)->set_types((yyvsp[-1].t_type));           /* Set types for constants */
       (yyval.t_type_list)->splice((yyval.t_type_list)->end(),*(yyvsp[0].t_type_list)); /* Join lists */ 
       delete (yyvsp[0].t_type_list);                   /* Delete (now empty) list */
   }
#line 2356 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 45:
#line 459 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {  
   // This parse needs to be excluded, we think (DPL&MF: 6/9/01)
       (yyval.t_type_list)= (yyvsp[-3].t_type_list);
       (yyval.t_type_list)->set_either_types((yyvsp[-1].t_type_list));
       (yyval.t_type_list)->splice((yyvsp[-3].t_type_list)->end(),*(yyvsp[0].t_type_list));
       delete (yyvsp[0].t_type_list);
   }
#line 2368 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 46:
#line 468 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_type_list)= (yyvsp[0].t_type_list); }
#line 2374 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 47:
#line 474 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_parameter_symbol_list)=(yyvsp[-1].t_parameter_symbol_list); (yyval.t_parameter_symbol_list)->push_back((yyvsp[0].t_const_symbol)); }
#line 2380 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 48:
#line 476 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_parameter_symbol_list)=(yyvsp[-2].t_parameter_symbol_list); (yyval.t_parameter_symbol_list)->push_back((yyvsp[0].t_var_symbol)); }
#line 2386 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 49:
#line 477 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_parameter_symbol_list)= new parameter_symbol_list;}
#line 2392 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 50:
#line 484 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_var_symbol)= current_analysis->var_tab_stack.top()->symbol_put((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2398 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 51:
#line 490 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_var_symbol)= current_analysis->var_tab_stack.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2404 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 52:
#line 494 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_const_symbol)= current_analysis->const_tab.symbol_get((yyvsp[0].cp)); delete [] (yyvsp[0].cp); }
#line 2410 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 53:
#line 498 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_const_symbol)= current_analysis->const_tab.symbol_put((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2416 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 54:
#line 503 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_type_list)= (yyvsp[-1].t_type_list); }
#line 2422 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 55:
#line 508 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_type)= current_analysis->pddl_type_tab.symbol_ref((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2428 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 56:
#line 515 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_type)= current_analysis->pddl_type_tab.symbol_ref((yyvsp[0].cp)); delete [] (yyvsp[0].cp);}
#line 2434 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 57:
#line 520 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_type_list)= (yyvsp[-1].t_type_list); (yyval.t_type_list)->push_back((yyvsp[0].t_type));}
#line 2440 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 58:
#line 521 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_type_list)= new pddl_type_list;}
#line 2446 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 59:
#line 526 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_type_list)= (yyvsp[-1].t_type_list); (yyval.t_type_list)->push_back((yyvsp[0].t_type));}
#line 2452 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 60:
#line 527 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_type_list)= new pddl_type_list;}
#line 2458 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 61:
#line 532 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-5].t_effect_lists);
	  (yyval.t_effect_lists)->assign_effects.push_back(new assignment((yyvsp[-2].t_func_term),E_ASSIGN,(yyvsp[-1].t_num_expression)));  
          requires(E_FLUENTS); 
	}
#line 2467 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 62:
#line 537 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[0].t_simple_effect)); }
#line 2473 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 63:
#line 539 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[0].t_simple_effect)); }
#line 2479 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 64:
#line 541 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[0].t_timed_effect)); }
#line 2485 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 65:
#line 543 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;}
#line 2491 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 66:
#line 548 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { requires(E_TIMED_INITIAL_LITERALS); 
   		(yyval.t_timed_effect)=new timed_initial_literal((yyvsp[-1].t_effect_lists),(yyvsp[-2].fval));}
#line 2498 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 67:
#line 553 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[0].t_effect_lists); (yyval.t_effect_lists)->append_effects((yyvsp[-1].t_effect_lists)); delete (yyvsp[-1].t_effect_lists);}
#line 2504 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 68:
#line 554 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[0].t_effect_lists); (yyval.t_effect_lists)->cond_effects.push_front((yyvsp[-1].t_cond_effect)); 
                                      requires(E_COND_EFFS);}
#line 2511 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 69:
#line 556 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[0].t_effect_lists); (yyval.t_effect_lists)->forall_effects.push_front((yyvsp[-1].t_forall_effect));
                                      requires(E_COND_EFFS);}
#line 2518 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 70:
#line 558 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists(); }
#line 2524 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 71:
#line 567 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[0].t_effect_lists);}
#line 2530 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 72:
#line 568 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2536 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 73:
#line 569 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2542 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 74:
#line 570 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->cond_effects.push_front((yyvsp[0].t_cond_effect));}
#line 2548 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 75:
#line 571 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->forall_effects.push_front((yyvsp[0].t_forall_effect));}
#line 2554 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 76:
#line 575 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists);}
#line 2560 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 77:
#line 576 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[0].t_effect_lists);}
#line 2566 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 78:
#line 581 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2572 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 79:
#line 583 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2578 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 80:
#line 585 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[0].t_assignment));
         requires(E_FLUENTS);}
#line 2585 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 81:
#line 591 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[0].t_simple_effect));}
#line 2591 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 82:
#line 592 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[0].t_simple_effect));}
#line 2597 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 83:
#line 593 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->assign_effects.push_back((yyvsp[0].t_assignment));
                                     requires(E_FLUENTS); }
#line 2604 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 84:
#line 595 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2610 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 85:
#line 600 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); }
#line 2616 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 86:
#line 602 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_effect_lists)=NULL;
	 log_error(E_FATAL,"Syntax error in (and ...)");
	}
#line 2624 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 87:
#line 610 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); }
#line 2630 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 88:
#line 615 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; 
          (yyval.t_effect_lists)->forall_effects.push_back(
	       new forall_effect((yyvsp[-1].t_effect_lists), (yyvsp[-3].t_var_symbol_list), current_analysis->var_tab_stack.pop())); 
          requires(E_COND_EFFS);}
#line 2639 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 89:
#line 620 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_effects.push_back(
	       new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)));
          requires(E_COND_EFFS); }
#line 2648 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 90:
#line 625 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_assign_effects.push_back(
	       new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)));
          requires(E_COND_EFFS); }
#line 2657 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 91:
#line 630 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=new effect_lists;
          (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[0].t_timed_effect)); }
#line 2664 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 92:
#line 633 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[0].t_assignment));
          requires(E_FLUENTS); }
#line 2672 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 93:
#line 639 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyvsp[-1].t_effect_lists)->append_effects((yyvsp[0].t_effect_lists)); delete (yyvsp[0].t_effect_lists); }
#line 2678 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 94:
#line 640 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2684 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 95:
#line 645 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect((yyvsp[-1].t_effect_lists),E_AT_START);}
#line 2690 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 96:
#line 647 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect((yyvsp[-1].t_effect_lists),E_AT_END);}
#line 2696 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 97:
#line 649 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression))); }
#line 2704 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 98:
#line 653 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression))); }
#line 2712 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 99:
#line 657 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_timed_effect)=NULL;
	log_error(E_FATAL,"Syntax error in timed effect"); }
#line 2719 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 100:
#line 663 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression))); }
#line 2727 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 101:
#line 667 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_timed_effect)=new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_timed_effect)->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression))); }
#line 2735 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 102:
#line 671 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_timed_effect)=NULL;
	log_error(E_FATAL,"Syntax error in conditional continuous effect"); }
#line 2742 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 103:
#line 677 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); }
#line 2748 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 104:
#line 682 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; 
          (yyval.t_effect_lists)->forall_effects.push_back(
	       new forall_effect((yyvsp[-1].t_effect_lists), (yyvsp[-3].t_var_symbol_list), current_analysis->var_tab_stack.pop())); 
          requires(E_COND_EFFS);}
#line 2757 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 105:
#line 687 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists;
	  (yyval.t_effect_lists)->cond_assign_effects.push_back(
	       new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)));
          requires(E_COND_EFFS); }
#line 2766 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 106:
#line 692 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=new effect_lists;
          (yyval.t_effect_lists)->timed_effects.push_back((yyvsp[0].t_timed_effect)); }
#line 2773 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 107:
#line 697 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyvsp[-1].t_effect_lists)->append_effects((yyvsp[0].t_effect_lists)); delete (yyvsp[0].t_effect_lists); }
#line 2779 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 108:
#line 698 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2785 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 109:
#line 702 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists);}
#line 2791 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 110:
#line 703 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[0].t_effect_lists);}
#line 2797 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 111:
#line 708 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->del_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2803 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 112:
#line 710 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->add_effects.push_front((yyvsp[0].t_simple_effect));}
#line 2809 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 113:
#line 712 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; (yyval.t_effect_lists)->assign_effects.push_front((yyvsp[0].t_assignment));
         requires(E_FLUENTS);}
#line 2816 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 114:
#line 718 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->del_effects.push_back((yyvsp[0].t_simple_effect));}
#line 2822 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 115:
#line 719 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->add_effects.push_back((yyvsp[0].t_simple_effect));}
#line 2828 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 116:
#line 720 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)= (yyvsp[-1].t_effect_lists); (yyval.t_effect_lists)->assign_effects.push_back((yyvsp[0].t_assignment));
                                     requires(E_FLUENTS); }
#line 2835 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 117:
#line 722 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2841 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 118:
#line 728 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_ASSIGN,(yyvsp[-1].t_expression)); }
#line 2847 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 119:
#line 730 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression)); }
#line 2853 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 120:
#line 732 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression)); }
#line 2859 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 121:
#line 734 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_UP,(yyvsp[-1].t_expression)); }
#line 2865 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 122:
#line 736 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_DOWN,(yyvsp[-1].t_expression)); }
#line 2871 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 123:
#line 741 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; 
         timed_effect * te = new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_effect_lists)->timed_effects.push_front(te);
         te->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression))); }
#line 2881 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 124:
#line 747 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=new effect_lists; 
         timed_effect * te = new timed_effect(new effect_lists,E_CONTINUOUS);
         (yyval.t_effect_lists)->timed_effects.push_front(te);
         te->effs->assign_effects.push_front(
	     new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression))); }
#line 2891 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 125:
#line 753 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists) = (yyvsp[-1].t_effect_lists);}
#line 2897 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 126:
#line 757 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists); (yyvsp[-1].t_effect_lists)->append_effects((yyvsp[0].t_effect_lists)); delete (yyvsp[0].t_effect_lists); }
#line 2903 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 127:
#line 758 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_effect_lists)= new effect_lists; }
#line 2909 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 128:
#line 762 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_expression);}
#line 2915 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 129:
#line 763 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression)= new special_val_expr(E_DURATION_VAR);
                    requires( E_DURATION_INEQUALITIES );}
#line 2922 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 130:
#line 765 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)=(yyvsp[0].t_num_expression); }
#line 2928 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 131:
#line 766 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= (yyvsp[0].t_func_term); }
#line 2934 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 132:
#line 771 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new plus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 2940 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 133:
#line 773 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new minus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 2946 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 134:
#line 775 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 2952 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 135:
#line 777 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new div_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); }
#line 2958 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 136:
#line 782 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal)= new conj_goal((yyvsp[-1].t_goal_list)); }
#line 2964 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 137:
#line 784 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal)= new timed_goal(new comparison((yyvsp[-4].t_comparison_op),
        			new special_val_expr(E_DURATION_VAR),(yyvsp[-1].t_expression)),E_AT_START); }
#line 2971 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 138:
#line 787 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal) = new timed_goal(new comparison((yyvsp[-5].t_comparison_op),
					new special_val_expr(E_DURATION_VAR),(yyvsp[-2].t_expression)),E_AT_START);}
#line 2978 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 139:
#line 790 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal) = new timed_goal(new comparison((yyvsp[-5].t_comparison_op),
					new special_val_expr(E_DURATION_VAR),(yyvsp[-2].t_expression)),E_AT_END);}
#line 2985 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 140:
#line 795 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_comparison_op)= E_LESSEQ; requires(E_DURATION_INEQUALITIES);}
#line 2991 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 141:
#line 796 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_comparison_op)= E_GREATEQ; requires(E_DURATION_INEQUALITIES);}
#line 2997 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 142:
#line 797 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_comparison_op)= E_EQUALS; }
#line 3003 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 143:
#line 805 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_expression); }
#line 3009 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 144:
#line 810 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[0].t_goal)); }
#line 3015 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 145:
#line 812 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal_list)= new goal_list; }
#line 3021 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 146:
#line 817 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[-1].t_proposition)); }
#line 3027 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 147:
#line 822 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[0].t_proposition)); }
#line 3033 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 148:
#line 829 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[-1].t_proposition)); }
#line 3039 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 149:
#line 834 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_simple_effect)= new simple_effect((yyvsp[0].t_proposition)); }
#line 3045 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 150:
#line 839 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_forall_effect)= new forall_effect((yyvsp[-1].t_effect_lists), (yyvsp[-3].t_var_symbol_list), current_analysis->var_tab_stack.pop());}
#line 3051 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 151:
#line 844 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_cond_effect)= new cond_effect((yyvsp[-2].t_goal),(yyvsp[-1].t_effect_lists)); }
#line 3057 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 152:
#line 849 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_ASSIGN,(yyvsp[-1].t_expression)); }
#line 3063 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 153:
#line 851 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_INCREASE,(yyvsp[-1].t_expression)); }
#line 3069 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 154:
#line 853 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_DECREASE,(yyvsp[-1].t_expression)); }
#line 3075 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 155:
#line 855 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_UP,(yyvsp[-1].t_expression)); }
#line 3081 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 156:
#line 857 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_assignment)= new assignment((yyvsp[-2].t_func_term),E_SCALE_DOWN,(yyvsp[-1].t_expression)); }
#line 3087 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 157:
#line 862 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new uminus_expression((yyvsp[-1].t_expression)); requires(E_FLUENTS); }
#line 3093 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 158:
#line 864 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new plus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_FLUENTS); }
#line 3099 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 159:
#line 866 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new minus_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_FLUENTS); }
#line 3105 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 160:
#line 868 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_FLUENTS); }
#line 3111 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 161:
#line 870 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new div_expression((yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); requires(E_FLUENTS); }
#line 3117 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 162:
#line 871 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)=(yyvsp[0].t_num_expression); }
#line 3123 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 163:
#line 872 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= (yyvsp[0].t_func_term); requires(E_FLUENTS); }
#line 3129 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 164:
#line 877 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression(new special_val_expr(E_HASHT),(yyvsp[-1].t_expression)); }
#line 3135 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 165:
#line 879 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-2].t_expression), new special_val_expr(E_HASHT)); }
#line 3141 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 166:
#line 881 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new special_val_expr(E_HASHT); }
#line 3147 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 167:
#line 886 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_num_expression)=new int_expression((yyvsp[0].ival));   }
#line 3153 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 168:
#line 887 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_num_expression)=new float_expression((yyvsp[0].fval)); }
#line 3159 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 169:
#line 891 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3165 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 170:
#line 894 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3171 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 171:
#line 896 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[0].cp)),
                            new parameter_symbol_list); delete [] (yyvsp[0].cp);}
#line 3178 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 172:
#line 914 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3184 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 173:
#line 916 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[-2].cp)), (yyvsp[-1].t_parameter_symbol_list)); delete [] (yyvsp[-2].cp); }
#line 3190 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 174:
#line 918 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_func_term)=new func_term( current_analysis->func_tab.symbol_get((yyvsp[0].cp)),
                            new parameter_symbol_list); delete [] (yyvsp[0].cp);}
#line 3197 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 175:
#line 923 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_GREATER; }
#line 3203 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 176:
#line 924 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_GREATEQ; }
#line 3209 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 177:
#line 925 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_LESS; }
#line 3215 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 178:
#line 926 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_LESSEQ; }
#line 3221 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 179:
#line 927 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_comparison_op)= E_EQUALS; }
#line 3227 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 180:
#line 940 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= (yyvsp[0].t_goal);}
#line 3233 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 181:
#line 947 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal) = new conj_goal((yyvsp[-1].t_goal_list));}
#line 3239 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 182:
#line 950 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());
        requires(E_UNIV_PRECS);}
#line 3246 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 183:
#line 956 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3252 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 184:
#line 958 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-2].cp),(yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3258 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 185:
#line 960 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new conj_goal((yyvsp[-1].t_goal_list));}
#line 3264 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 186:
#line 963 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal)= new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_con_goal),current_analysis->var_tab_stack.pop());
                requires(E_UNIV_PRECS);}
#line 3271 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 187:
#line 966 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = (yyvsp[0].t_con_goal);}
#line 3277 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 188:
#line 971 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3283 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 189:
#line 973 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new preference((yyvsp[-2].cp),(yyvsp[-1].t_con_goal));requires(E_PREFERENCES);}
#line 3289 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 190:
#line 975 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new conj_goal((yyvsp[-1].t_goal_list));}
#line 3295 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 191:
#line 978 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal)= new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_con_goal),current_analysis->var_tab_stack.pop());
                requires(E_UNIV_PRECS);}
#line 3302 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 192:
#line 984 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyvsp[-1].t_goal_list)->push_back((yyvsp[0].t_con_goal));}
#line 3308 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 193:
#line 986 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[0].t_con_goal));}
#line 3314 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 194:
#line 991 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new preference((yyvsp[-1].t_goal)); requires(E_PREFERENCES);}
#line 3320 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 195:
#line 993 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new preference((yyvsp[-2].cp),(yyvsp[-1].t_goal)); requires(E_PREFERENCES);}
#line 3326 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 196:
#line 997 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)=(yyvsp[0].t_goal);}
#line 3332 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 197:
#line 1002 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list) = (yyvsp[-1].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[0].t_con_goal));}
#line 3338 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 198:
#line 1004 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list) = new goal_list; (yyval.t_goal_list)->push_back((yyvsp[0].t_con_goal));}
#line 3344 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 199:
#line 1009 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal)= new conj_goal((yyvsp[-1].t_goal_list));}
#line 3350 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 200:
#line 1011 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new qfied_goal(E_FORALL,(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_con_goal),current_analysis->var_tab_stack.pop());
        requires(E_UNIV_PRECS);}
#line 3357 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 201:
#line 1014 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ATEND,(yyvsp[-1].t_goal));}
#line 3363 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 202:
#line 1016 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ALWAYS,(yyvsp[-1].t_goal));}
#line 3369 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 203:
#line 1018 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIME,(yyvsp[-1].t_goal));}
#line 3375 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 204:
#line 1020 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_WITHIN,(yyvsp[-1].t_goal),NULL,(yyvsp[-2].t_num_expression)->double_value(),0.0);delete (yyvsp[-2].t_num_expression);}
#line 3381 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 205:
#line 1022 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ATMOSTONCE,(yyvsp[-1].t_goal));}
#line 3387 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 206:
#line 1024 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIMEAFTER,(yyvsp[-1].t_goal),(yyvsp[-2].t_goal));}
#line 3393 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 207:
#line 1026 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_SOMETIMEBEFORE,(yyvsp[-1].t_goal),(yyvsp[-2].t_goal));}
#line 3399 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 208:
#line 1028 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_ALWAYSWITHIN,(yyvsp[-1].t_goal),(yyvsp[-2].t_goal),(yyvsp[-3].t_num_expression)->double_value(),0.0);delete (yyvsp[-3].t_num_expression);}
#line 3405 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 209:
#line 1030 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_HOLDDURING,(yyvsp[-1].t_goal),NULL,(yyvsp[-2].t_num_expression)->double_value(),(yyvsp[-3].t_num_expression)->double_value());delete (yyvsp[-3].t_num_expression);delete (yyvsp[-2].t_num_expression);}
#line 3411 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 210:
#line 1032 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = new constraint_goal(E_HOLDAFTER,(yyvsp[-1].t_goal),NULL,0.0,(yyvsp[-2].t_num_expression)->double_value());delete (yyvsp[-2].t_num_expression);}
#line 3417 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 211:
#line 1037 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new simple_goal((yyvsp[0].t_proposition),E_POS);}
#line 3423 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 212:
#line 1039 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new neg_goal((yyvsp[-1].t_goal));simple_goal * s = dynamic_cast<simple_goal *>((yyvsp[-1].t_goal));
       if(s && s->getProp()->head->getName()=="=") {requires(E_EQUALITY);} 
       else{requires(E_NEGATIVE_PRECONDITIONS);};}
#line 3431 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 213:
#line 1043 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new conj_goal((yyvsp[-1].t_goal_list));}
#line 3437 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 214:
#line 1045 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new disj_goal((yyvsp[-1].t_goal_list));
        requires(E_DISJUNCTIVE_PRECONDS);}
#line 3444 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 215:
#line 1048 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new imply_goal((yyvsp[-2].t_goal),(yyvsp[-1].t_goal));
        requires(E_DISJUNCTIVE_PRECONDS);}
#line 3451 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 216:
#line 1052 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new qfied_goal((yyvsp[-5].t_quantifier),(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());}
#line 3457 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 217:
#line 1055 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new qfied_goal((yyvsp[-5].t_quantifier),(yyvsp[-3].t_var_symbol_list),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());}
#line 3463 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 218:
#line 1057 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new comparison((yyvsp[-3].t_comparison_op),(yyvsp[-2].t_expression),(yyvsp[-1].t_expression)); 
        requires(E_FLUENTS);}
#line 3470 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 219:
#line 1063 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyvsp[-1].t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3476 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 220:
#line 1065 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3482 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 221:
#line 1070 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyvsp[-1].t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3488 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 222:
#line 1072 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal_list)= new goal_list; (yyval.t_goal_list)->push_back((yyvsp[0].t_goal));}
#line 3494 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 223:
#line 1082 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_quantifier)=E_FORALL; 
        current_analysis->var_tab_stack.push(
        		current_analysis->buildForallTab());}
#line 3502 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 224:
#line 1089 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_quantifier)=E_EXISTS;
        current_analysis->var_tab_stack.push(
        	current_analysis->buildExistsTab());}
#line 3510 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 225:
#line 1096 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_proposition)=new proposition((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_parameter_symbol_list));}
#line 3516 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 226:
#line 1101 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_proposition) = new proposition((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_var_symbol_list));}
#line 3522 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 227:
#line 1106 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_proposition)=new proposition((yyvsp[-2].t_pred_symbol),(yyvsp[-1].t_parameter_symbol_list));}
#line 3528 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 228:
#line 1111 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pred_decl_list)= (yyvsp[-1].t_pred_decl_list);}
#line 3534 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 229:
#line 1113 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_pred_decl_list)=NULL;
	 log_error(E_FATAL,"Syntax error in (:predicates ...)");
	}
#line 3542 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 230:
#line 1120 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_func_decl_list)= (yyvsp[-1].t_func_decl_list);}
#line 3548 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 231:
#line 1122 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_func_decl_list)=NULL;
	 log_error(E_FATAL,"Syntax error in (:functions ...)");
	}
#line 3556 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 232:
#line 1129 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = (yyvsp[-1].t_con_goal);}
#line 3562 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 233:
#line 1131 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_con_goal)=NULL;
      log_error(E_FATAL,"Syntax error in (:constraints ...)");
      }
#line 3570 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 234:
#line 1138 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_con_goal) = (yyvsp[-1].t_con_goal);}
#line 3576 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 235:
#line 1140 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_con_goal)=NULL;
      log_error(E_FATAL,"Syntax error in (:constraints ...)");
      }
#line 3584 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 236:
#line 1146 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_structure_store)=(yyvsp[-1].t_structure_store); (yyval.t_structure_store)->push_back((yyvsp[0].t_structure_def)); }
#line 3590 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 237:
#line 1147 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_structure_store)= new structure_store; (yyval.t_structure_store)->push_back((yyvsp[0].t_structure_def)); }
#line 3596 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 238:
#line 1151 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_action_def); }
#line 3602 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 239:
#line 1152 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_event_def); requires(E_TIME); }
#line 3608 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 240:
#line 1153 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_process_def); requires(E_TIME); }
#line 3614 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 241:
#line 1154 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_durative_action_def); requires(E_DURATIVE_ACTIONS); }
#line 3620 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 242:
#line 1155 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_structure_def)= (yyvsp[0].t_derivation_rule); requires(E_DERIVED_PREDICATES);}
#line 3626 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 243:
#line 1159 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_dummy)= 0; 
    	current_analysis->var_tab_stack.push(
    					current_analysis->buildRuleTab());}
#line 3634 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 244:
#line 1170 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_derivation_rule) = new derivation_rule((yyvsp[-2].t_proposition),(yyvsp[-1].t_goal),current_analysis->var_tab_stack.pop());}
#line 3640 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 245:
#line 1182 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_action_def)= current_analysis->buildAction(current_analysis->op_tab.symbol_put((yyvsp[-9].cp)),
			(yyvsp[-6].t_var_symbol_list),(yyvsp[-3].t_goal),(yyvsp[-1].t_effect_lists),
			current_analysis->var_tab_stack.pop()); delete [] (yyvsp[-9].cp); }
#line 3648 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 246:
#line 1186 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
	 log_error(E_FATAL,"Syntax error in action declaration.");
	 (yyval.t_action_def)= NULL; }
#line 3656 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 247:
#line 1199 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_event_def)= current_analysis->buildEvent(current_analysis->op_tab.symbol_put((yyvsp[-9].cp)),
		   (yyvsp[-6].t_var_symbol_list),(yyvsp[-3].t_goal),(yyvsp[-1].t_effect_lists),
		   current_analysis->var_tab_stack.pop()); delete [] (yyvsp[-9].cp);}
#line 3664 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 248:
#line 1204 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
	 log_error(E_FATAL,"Syntax error in event declaration.");
	 (yyval.t_event_def)= NULL; }
#line 3672 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 249:
#line 1216 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_process_def)= current_analysis->buildProcess(current_analysis->op_tab.symbol_put((yyvsp[-9].cp)),
		     (yyvsp[-6].t_var_symbol_list),(yyvsp[-3].t_goal),(yyvsp[-1].t_effect_lists),
                     current_analysis->var_tab_stack.pop()); delete [] (yyvsp[-9].cp);}
#line 3680 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 250:
#line 1220 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
	 log_error(E_FATAL,"Syntax error in process declaration.");
	 (yyval.t_process_def)= NULL; }
#line 3688 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 251:
#line 1232 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_durative_action_def)= (yyvsp[-1].t_durative_action_def);
      (yyval.t_durative_action_def)->name= current_analysis->op_tab.symbol_put((yyvsp[-8].cp));
      (yyval.t_durative_action_def)->symtab= current_analysis->var_tab_stack.pop();
      (yyval.t_durative_action_def)->parameters= (yyvsp[-5].t_var_symbol_list);
      (yyval.t_durative_action_def)->dur_constraint= (yyvsp[-2].t_goal); 
      delete [] (yyvsp[-8].cp);
    }
#line 3700 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 252:
#line 1241 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
	 log_error(E_FATAL,"Syntax error in durative-action declaration.");
	 (yyval.t_durative_action_def)= NULL; }
#line 3708 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 253:
#line 1248 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_durative_action_def)=(yyvsp[-2].t_durative_action_def); (yyval.t_durative_action_def)->effects=(yyvsp[0].t_effect_lists);}
#line 3714 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 254:
#line 1250 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_durative_action_def)=(yyvsp[-2].t_durative_action_def); (yyval.t_durative_action_def)->precondition=(yyvsp[0].t_goal);}
#line 3720 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 255:
#line 1251 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_durative_action_def)= current_analysis->buildDurativeAction();}
#line 3726 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 256:
#line 1256 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal)=(yyvsp[0].t_goal); }
#line 3732 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 257:
#line 1258 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal)= new conj_goal((yyvsp[-1].t_goal_list)); }
#line 3738 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 258:
#line 1263 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal_list)=(yyvsp[-1].t_goal_list); (yyval.t_goal_list)->push_back((yyvsp[0].t_goal)); }
#line 3744 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 259:
#line 1265 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_goal_list)= new goal_list; }
#line 3750 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 260:
#line 1270 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new timed_goal((yyvsp[-1].t_goal),E_AT_START);}
#line 3756 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 261:
#line 1272 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new timed_goal((yyvsp[-1].t_goal),E_AT_END);}
#line 3762 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 262:
#line 1274 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)= new timed_goal((yyvsp[-1].t_goal),E_OVER_ALL);}
#line 3768 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 263:
#line 1276 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {timed_goal * tg = dynamic_cast<timed_goal *>((yyvsp[-1].t_goal));
		(yyval.t_goal) = new timed_goal(new preference((yyvsp[-2].cp),tg->clearGoal()),tg->getTime());
			delete tg;
			requires(E_PREFERENCES);}
#line 3777 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 264:
#line 1281 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal) = new preference((yyvsp[-1].t_goal));requires(E_PREFERENCES);}
#line 3783 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 265:
#line 1285 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_dummy)= 0; current_analysis->var_tab_stack.push(
    				current_analysis->buildOpTab());}
#line 3790 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 266:
#line 1290 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_EQUALITY;}
#line 3796 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 267:
#line 1291 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_STRIPS;}
#line 3802 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 268:
#line 1293 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_TYPING;}
#line 3808 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 269:
#line 1295 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_NEGATIVE_PRECONDITIONS;}
#line 3814 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 270:
#line 1297 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_DISJUNCTIVE_PRECONDS;}
#line 3820 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 271:
#line 1298 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_EXT_PRECS;}
#line 3826 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 272:
#line 1299 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_UNIV_PRECS;}
#line 3832 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 273:
#line 1300 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_COND_EFFS;}
#line 3838 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 274:
#line 1301 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_FLUENTS;}
#line 3844 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 275:
#line 1303 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_DURATIVE_ACTIONS;}
#line 3850 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 276:
#line 1304 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_TIME |
                      E_FLUENTS |
                      E_DURATIVE_ACTIONS; }
#line 3858 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 277:
#line 1308 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_STRIPS |
		      E_TYPING | 
		      E_NEGATIVE_PRECONDITIONS |
		      E_DISJUNCTIVE_PRECONDS |
		      E_EQUALITY |
		      E_EXT_PRECS |
		      E_UNIV_PRECS |
		      E_COND_EFFS;}
#line 3871 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 278:
#line 1317 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_EXT_PRECS |
		      E_UNIV_PRECS;}
#line 3878 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 279:
#line 1321 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_DURATION_INEQUALITIES;}
#line 3884 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 280:
#line 1324 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag)= E_CONTINUOUS_EFFECTS;}
#line 3890 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 281:
#line 1326 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_DERIVED_PREDICATES;}
#line 3896 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 282:
#line 1328 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_TIMED_INITIAL_LITERALS;}
#line 3902 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 283:
#line 1330 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_PREFERENCES;}
#line 3908 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 284:
#line 1332 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_pddl_req_flag) = E_CONSTRAINTS;}
#line 3914 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 285:
#line 1334 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {log_error(E_WARNING,"Unrecognised requirements declaration ");
       (yyval.t_pddl_req_flag)= 0; delete [] (yyvsp[0].cp);}
#line 3921 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 286:
#line 1340 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[-1].t_const_symbol_list);}
#line 3927 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 287:
#line 1344 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_type_list)=(yyvsp[-1].t_type_list); requires(E_TYPING);}
#line 3933 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 288:
#line 1354 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[-1].t_problem); (yyval.t_problem)->name = (yyvsp[-7].cp); (yyval.t_problem)->domain_name = (yyvsp[-3].cp);
		if (types_used && !types_defined) {
			yyerrok; log_error(E_FATAL,"Syntax error in problem file - types used, but no :types section in domain file."); 
		}

	}
#line 3944 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 289:
#line 1361 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; (yyval.t_problem)=NULL;
       	log_error(E_FATAL,"Syntax error in problem definition."); }
#line 3951 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 290:
#line 1367 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->req= (yyvsp[-1].t_pddl_req_flag);}
#line 3957 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 291:
#line 1368 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->objects= (yyvsp[-1].t_const_symbol_list);}
#line 3963 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 292:
#line 1369 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->initial_state= (yyvsp[-1].t_effect_lists);}
#line 3969 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 293:
#line 1370 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->the_goal= (yyvsp[-1].t_goal);}
#line 3975 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 294:
#line 1372 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->constraints = (yyvsp[-1].t_con_goal);}
#line 3981 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 295:
#line 1373 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->metric= (yyvsp[-1].t_metric);}
#line 3987 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 296:
#line 1374 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=(yyvsp[0].t_problem); (yyval.t_problem)->length= (yyvsp[-1].t_length_spec);}
#line 3993 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 297:
#line 1375 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_problem)=new problem;}
#line 3999 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 298:
#line 1378 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_const_symbol_list)=(yyvsp[-1].t_const_symbol_list);}
#line 4005 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 299:
#line 1381 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_effect_lists)=(yyvsp[-1].t_effect_lists);}
#line 4011 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 300:
#line 1384 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.vtab) = current_analysis->buildOpTab();}
#line 4017 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 301:
#line 1387 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_goal)=(yyvsp[-1].t_goal);delete (yyvsp[-2].vtab);}
#line 4023 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 302:
#line 1392 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_metric)= new metric_spec((yyvsp[-2].t_optimization),(yyvsp[-1].t_expression)); }
#line 4029 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 303:
#line 1394 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {yyerrok; 
        log_error(E_FATAL,"Syntax error in metric declaration.");
        (yyval.t_metric)= NULL; }
#line 4037 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 304:
#line 1401 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_length_spec)= new length_spec(E_BOTH,(yyvsp[-3].ival),(yyvsp[-1].ival));}
#line 4043 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 305:
#line 1404 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_length_spec) = new length_spec(E_SERIAL,(yyvsp[-1].ival));}
#line 4049 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 306:
#line 1408 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_length_spec) = new length_spec(E_PARALLEL,(yyvsp[-1].ival));}
#line 4055 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 307:
#line 1414 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_optimization)= E_MINIMIZE;}
#line 4061 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 308:
#line 1415 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_optimization)= E_MAXIMIZE;}
#line 4067 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 309:
#line 1420 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[-1].t_expression);}
#line 4073 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 310:
#line 1421 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_func_term);}
#line 4079 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 311:
#line 1422 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression)= (yyvsp[0].t_num_expression);}
#line 4085 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 312:
#line 1423 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new special_val_expr(E_TOTAL_TIME); }
#line 4091 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 313:
#line 1425 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression) = new violation_term((yyvsp[-1].cp));}
#line 4097 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 314:
#line 1426 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new special_val_expr(E_TOTAL_TIME); }
#line 4103 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 315:
#line 1430 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new plus_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4109 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 316:
#line 1431 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new minus_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4115 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 317:
#line 1432 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new mul_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4121 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 318:
#line 1433 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    { (yyval.t_expression)= new div_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression)); }
#line 4127 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 319:
#line 1437 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression) = (yyvsp[0].t_expression);}
#line 4133 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 320:
#line 1439 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression) = new plus_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression));}
#line 4139 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 321:
#line 1443 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression) = (yyvsp[0].t_expression);}
#line 4145 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 322:
#line 1445 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_expression) = new mul_expression((yyvsp[-1].t_expression),(yyvsp[0].t_expression));}
#line 4151 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 323:
#line 1451 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_plan)= (yyvsp[0].t_plan); 
         (yyval.t_plan)->push_front((yyvsp[-1].t_step)); }
#line 4158 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 324:
#line 1454 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_plan) = (yyvsp[0].t_plan);(yyval.t_plan)->insertTime((yyvsp[-1].fval));}
#line 4164 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 325:
#line 1456 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_plan) = (yyvsp[0].t_plan);(yyval.t_plan)->insertTime((yyvsp[-1].ival));}
#line 4170 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 326:
#line 1458 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_plan)= new plan;}
#line 4176 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 327:
#line 1463 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_step)=(yyvsp[0].t_step); 
         (yyval.t_step)->start_time_given=1; 
         (yyval.t_step)->start_time=(yyvsp[-2].fval);}
#line 4184 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 328:
#line 1467 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_step)=(yyvsp[0].t_step);
	 (yyval.t_step)->start_time_given=0;}
#line 4191 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 329:
#line 1473 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_step)= (yyvsp[-3].t_step); 
	 (yyval.t_step)->duration_given=1;
         (yyval.t_step)->duration= (yyvsp[-1].fval);}
#line 4199 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 330:
#line 1477 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_step)= (yyvsp[0].t_step);
         (yyval.t_step)->duration_given=0;}
#line 4206 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 331:
#line 1483 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.t_step)= new plan_step( 
              current_analysis->op_tab.symbol_get((yyvsp[-2].cp)), 
	      (yyvsp[-1].t_const_symbol_list)); delete [] (yyvsp[-2].cp);
      }
#line 4215 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 332:
#line 1490 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.fval)= (yyvsp[0].fval);}
#line 4221 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;

  case 333:
#line 1491 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1646  */
    {(yyval.fval)= (float) (yyvsp[0].ival);}
#line 4227 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
    break;


#line 4231 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.cpp" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1494 "/home/royboy18/robochef/planners/marvin2/marvin2/parsing/pddl+.yacc" /* yacc.c:1906  */


#include <cstdio>
#include <iostream>
int line_no= 1;
using std::istream;
#include "lex.yy.cc"

namespace VAL {
extern yyFlexLexer* yfl;
};


int yyerror(const char * s)
{
    return 0;
}

int yylex()
{
    return yfl->yylex();
}
#include "fixyywrap.h"

