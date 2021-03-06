#!/bin/bash

TAB='\t';
NULO='\\N';
LIDOS='lidos/';
SQLCOPY='sqlCopy/';


FILE=$1;
echo "COPY financeiro.*************(***********) FROM stdin WITH NULL AS '${NULO}';" >> ${FILE}.sql;

#file_length=`wc -l '${FILE}' | cut -c1-5`;


cat ${FILE} | grep ^[^SC*] | while read LINHA;
do 

    ITCOUSUARIO=${CONSTANTE:0:11};
    
    ITCOTERMINALUSUARIO=${LINHA:11:8};

    ITDATRANSACAO=${LINHA:19:8};

    ITHOTRANSACAO=${LINHA:27:4};

    ITCOUGOPERADOR=${LINHA:31:6};

    GRUGGESTAOANNUMEROLB=${LINHA:37:23};

    ITCOBANCO=${LINHA:60:3};

    ITVABANCO=${LINHA:63:17};

    GRANNUOBCANCELAMENTO=${LINHA:80:12};

    ITCOCIT=${LINHA:92:3};

    ITCOAGENCIAC=${LINHA:95:4};

    ITNODESCRICAOCIT=${LINHA:99:25};

    GRUGGESTAOCANCELAMENTO=${LINHA:124:11};

    ITDASAQUEBACEN=${LINHA:135:8};

    ITVACANCELAMENTO=${LINHA:143:17};

    ITNUOPERACAOSPB=${LINHA:160:23};

    FILLER=${LINHA:183:17};


    echo -e $ITCOUSUARIO${TAB}$ITCOTERMINALUSUARIO${TAB}$ITDATRANSACAO${TAB}$ITHOTRANSACAO${TAB}$ITCOUGOPERADOR${TAB}$GRUGGESTAOANNUMEROLB${TAB}$ITCOBANCO${TAB}$ITVABANCO${TAB}$GRANNUOBCANCELAMENTO${TAB}$ITCOCIT${TAB}$ITCOAGENCIAC${TAB}$ITNODESCRICAOCIT${TAB}$GRUGGESTAOCANCELAMENTO${TAB}$ITDASAQUEBACEN${TAB}$ITVACANCELAMENTO${TAB}$ITNUOPERACAOSPB${TAB}$FILLER >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;

