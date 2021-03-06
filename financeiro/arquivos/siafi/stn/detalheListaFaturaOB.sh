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

    GRUGGESTAOANNUMEROLF=${LINHA:37:23};

    ITNUSEQUENCIAL=${LINHA:60:6};

    ITVAITEM=${LINHA:66:17};

    ITINTIPOFATURA=${LINHA:83:1};

    ITNUCODIGOBARRA=${LINHA:84:48};

    ITNUCODIGOBARRACALCULADO=${LINHA:132:44};

    GRANNUOBCANCELAMENTOLF=${LINHA:176:12};

    GRANNUOBRESTABELECIMENTOLF=${LINHA:188:12};

    GRUGGESTAOCANCELAMENTOLF=${LINHA:200:11};

    ITINSITUACAO=${LINHA:211:1};

    ITCOFAVORECIDO=${LINHA:212:14};

    ITVADESCONTO=${LINHA:226:17};

    ITVAMORAMULTA=${LINHA:243:17};

    ITVAOUTRASDEDUCOES=${LINHA:260:17};

    ITVAOUTROSACRESCIMOS=${LINHA:277:17};

    ITINTIPOENTRADADADOS=${LINHA:294:1};

    ITINCNPJBB=${LINHA:295:1};

    FILLER=${LINHA:296:104};


    echo -e $ITCOUSUARIO${TAB}$ITCOTERMINALUSUARIO${TAB}$ITDATRANSACAO${TAB}$ITHOTRANSACAO${TAB}$ITCOUGOPERADOR${TAB}$GRUGGESTAOANNUMEROLF${TAB}$ITNUSEQUENCIAL${TAB}$ITVAITEM${TAB}$ITINTIPOFATURA${TAB}$ITNUCODIGOBARRA${TAB}$ITNUCODIGOBARRACALCULADO${TAB}$GRANNUOBCANCELAMENTOLF${TAB}$GRANNUOBRESTABELECIMENTOLF${TAB}$GRUGGESTAOCANCELAMENTOLF${TAB}$ITINSITUACAO${TAB}$ITCOFAVORECIDO${TAB}$ITVADESCONTO${TAB}$ITVAMORAMULTA${TAB}$ITVAOUTRASDEDUCOES${TAB}$ITVAOUTROSACRESCIMOS${TAB}$ITINTIPOENTRADADADOS${TAB}$ITINCNPJBB${TAB}$FILLER >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;

