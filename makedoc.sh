#!/bin/bash
export CHVERSION=2.6.6

function makeclean() {
  rm *.log *.chm *.pdf *.tar.gz 2> /dev/null
}

function makechm() {
  rm html/* 2> /dev/null
  cp ../common/rsc/logo_small.png html/logo_small.png
  if ! doxygen Doxyfile_chm 2> tmp.log
  then
    echo "Doxygen failed"
    exit
  fi
  cat tmp.log | fgrep -v "warning: return type of member" > chibios_rt_${CHVERSION}_${CHNAME}_chm2.log
  if [ -s chibios_rt_${CHVERSION}_${CHNAME}_chm2.log ]
  then
    echo "  * Doxigen warnings or errors"
    exit
  fi
  rm tmp.log 2> /dev/null
  cp html/refman.chm chibios_rt_${CHVERSION}_${CHNAME}.chm
  rm html/* 2> /dev/null
  echo "  - CHM generated"
}

function makepdf() {
  rm latex/* 2> /dev/null
  cp ../common/rsc/chibios.sty ./latex
  if ! doxygen Doxyfile_pdf 2> /dev/null
  then
    echo "Doxygen failed"
    exit
  fi
  cp ../common/rsc/Makefile ./latex
  cd latex
  # Patches a Doxygen bug.
  if grep "input{hierarchy}" < refman.tex > /dev/null
  then
    cp refman.tex refman2.tex
    sed -e '0,/input{hierarchy}/s/Data Structure/Class Hierarchy/' < refman2.tex > refman.tex
  fi
  # PDF generation.
  if ! make all 1> ../chibios_rt_${CHVERSION}_${CHNAME}_pdf1.log 2> ../chibios_rt_${CHVERSION}_${CHNAME}_pdf2.log
  then
    echo "Make failed"
    exit
  fi
  cd ..
  cp latex/refman.pdf chibios_rt_${CHVERSION}_${CHNAME}.pdf
  rm latex/* 2> /dev/null
  echo "  - PDF generated"
}

function makearchive() {
  tar -pczf chibios_rt_${CHVERSION}_${CHNAME}.tar.gz chibios_rt_${CHVERSION}_${CHNAME}.chm chibios_rt_${CHVERSION}_${CHNAME}.pdf
}

export CHNAME=kernel_hal_rm
echo doc_${CHNAME}
cd doc_${CHNAME}
makeclean
makechm
makepdf
makearchive
cd ..

export CHNAME=port_arm79_gcc_rm
echo doc_${CHNAME}
cd doc_${CHNAME}
makeclean
makechm
makepdf
makearchive
cd ..

export CHNAME=port_armcmx_gcc_rm
echo doc_${CHNAME}
cd doc_${CHNAME}
makeclean
makechm
makepdf
makearchive
cd ..

export CHNAME=port_armcmx_iar_rm
echo doc_${CHNAME}
cd doc_${CHNAME}
makeclean
makechm
makepdf
makearchive
cd ..

export CHNAME=port_armcmx_rvct_rm
echo doc_${CHNAME}
cd doc_${CHNAME}
makeclean
makechm
makepdf
makearchive
cd ..

export CHNAME=port_ppc_gcc_rm
echo doc_${CHNAME}
cd doc_${CHNAME}
makeclean
makechm
makepdf
makearchive
cd ..
