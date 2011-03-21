#!/bin/sh

case "@QT4_FOUND@" in
  YES|TRUE|true|yes|on|ON) haveQt4=true ;;
  *) haveQt4=false ;;
esac
case "@LARRABEE_FOUND@" in
  YES|TRUE|true|yes|on|ON) haveLrbni=true ;;
  *) haveLrbni=false ;;
esac
case "@USE_SSE2@" in
  YES|TRUE|true|yes|on|ON) haveSse=true ;;
  *) haveSse=false ;;
esac
case "@USE_AVX@" in
  YES|TRUE|true|yes|on|ON) haveAvx=true ;;
  *) haveAvx=false ;;
esac

cd `dirname $0`
resultsDir="benchmark-all-`hostname`-`date '+%Y-%m-%d-%H-%M-%S'`"
mkdir $resultsDir || exit
echo "Storing benchmark results to $PWD/$resultsDir"

srcdir='@Vc_SOURCE_DIR@'
rev=`cut -d' ' -f2 "$srcdir/.git/HEAD"`
branch=${rev##*/}
rev=`cat "$srcdir/.git/$rev"`

CXX='@CMAKE_CXX_COMPILER@'

cat > $resultsDir/metadata <<EOF
build type	: @CMAKE_BUILD_TYPE@
compiler	: `$CXX --version|head -n1`
strict aliasing	: @ENABLE_STRICT_ALIASING@
fast math	: @FAST_MATH_BENCHMARK@
realtime	: @REALTIME_BENCHMARKS@
target arch	: @TARGET_ARCHITECTURE@
flags		: `echo '@CMAKE_CXX_FLAGS@'|sed 's/-W[^ ]*//g'|tr -s '[:space:]'`
Vc branch	: $branch
Vc revision	: $rev
hostname	: `hostname`
machine		: `uname -m`
`grep -m1 -B2 'model name' /proc/cpuinfo`
EOF

executeBench()
{
  name=${1}_${2}
  if test -x ./$name; then
    outfile=$resultsDir/$name
    $haveAvx && test "$2" != "lrb" && outfile=${outfile}-mavx
    outfile=${outfile}.dat
    printf "%22s -o %s\n" "$name" "$outfile"
    ./$name -o $outfile >/dev/null 2>&1
  else
    printf "%22s SKIPPED\n" "$name"
  fi
}

if which benchmarking.sh >/dev/null; then
  echo "Calling 'benchmarking.sh start' to disable powermanagement and Turbo-Mode"
  benchmarking.sh start
fi

for bench in \
  flops arithmetics gather mask compare math memio dhryrock whetrock mandelbrotbench
do
  executeBench $bench scalar
  $haveSse && executeBench $bench sse
  $haveAvx && executeBench $bench avx
  $haveLrbni && executeBench $bench lrb
done

if which benchmarking.sh >/dev/null; then
  echo "Calling 'benchmarking.sh stop' to re-enable powermanagement and Turbo-Mode"
  benchmarking.sh stop
fi

# vim: sw=2 et
