<?php
ini_set("memory_limit","1024M");
set_time_limit(0);

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

$monstrarCabecalho = true;

// monta o sql, agrupador e coluna do relatório
$sql       = monta_sql();
$agrupador = monta_agp();
$coluna    = monta_col();

$dados     = $db->carregar( $sql );
$dados     = $dados ? $dados : array();

$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTotNivel(false);
$rel->setEspandir(false);

// Gera o XLS do relatório
if ( $_REQUEST['req'] == 'xls' ){
    ob_clean();
    $nomeDoArquivoXls = 'relatorio';
    echo $rel->getRelatorioXls();
    die();
}

?>
<!DOCTYPE html>
<html>
    <head>
        <title> Minc </title>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
    </head>
    <body>
        <center>
            <!--  Cabeçalho Brasão -->
            <?php echo monta_cabecalho_relatorio( '100',$monstrarCabecalho ); ?>
        </center>

        <!--  Monta o Relatório -->
        <?php echo $rel->getRelatorio(); ?>

    </body>
</html>
<?php
function monta_sql(){
    $where = array();
    global $db;

    $join = monta_join();
    $campos = monta_colunas();
    $sql = "SELECT ".implode(', ', $campos)."
              FROM emendas.emenda eme
              ".implode(' ', $join)."
             where eme.emestatus = 'A'". monta_where();
//    ver($sql,d);
    return $sql;
}

function monta_where(){
    $where = '';
    foreach($_REQUEST['filtro'] as $key=>$value){
        if (!empty($value))
            $where .= " and ".$key." = '".$value."'";
    }
    return $where;
}
function monta_agp(){
    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
    $agrupador = ($agrupador ? $agrupador : array());

    $agp = array("agrupador"      => array());

    foreach ( $agrupador as $val ){
        switch( $val ){
            case "emeid":
                array_push($agp['agrupador'], array("campo" => "emeid","label" => "ID Emenda","type" => "string"));
            break;
            case "emenumero":
                array_push($agp['agrupador'], array("campo" => "emenumero","label" => "Número","type" => "string"));
            break;
            case "autcod":
                array_push($agp['agrupador'], array("campo" => "autcod","label" => "Cod. Autor","type" => "string"));
            break;
            case "autnome":
                array_push($agp['agrupador'], array("campo" => "autnome","label" => "Autor","type" => "string"));
            break;
            case "autsexo":
                array_push($agp['agrupador'], array("campo" => "autsexo","label" => "Gênero","type" => "string"));
            break;
            case "atpdsc":
                array_push($agp['agrupador'], array("campo" => "atpdsc","label" => "Tipo Parlamentar","type" => "string"));
            break;
            case "estuf":
                array_push($agp['agrupador'], array("campo" => "estuf","label" => "UF Parlamentar","type" => "string"));
            break;
            case "parsigla":
                array_push($agp['agrupador'], array("campo" => "parsigla","label" => "Partido","type" => "string"));
            break;
            case "unocod":
                array_push($agp['agrupador'], array("campo" => "unocod","label" => "Cod. Unidade","type" => "string"));
            break;
            case "unosigla":
                array_push($agp['agrupador'], array("campo" => "unosigla","label" => "Unidade","type" => "string"));
            break;
            case "funcional":
                array_push($agp['agrupador'], array("campo" => "funcional","label" => "Funcional","type" => "string"));
            break;
            case "benid":
                array_push($agp['agrupador'], array("campo" => "benid","label" => "Id Beneficiario","type" => "string"));
            break;
            case "suocod":
                array_push($agp['agrupador'], array("campo" => "suocod","label" => "Código Subunidade","type" => "string"));
            break;
            case "suonome":
                array_push($agp['agrupador'], array("campo" => "suonome","label" => "Subunidade","type" => "string"));
            break;
            case "suocoddelegada":
                array_push($agp['agrupador'], array("campo" => "suocoddelegada","label" => "Código Subunidade delegada","type" => "string"));
            break;
            case "suonomedelegada":
                array_push($agp['agrupador'], array("campo" => "suonomedelegada","label" => "Subunidade delegada","type" => "string"));
            break;
            case "mdedsc":
                array_push($agp['agrupador'], array("campo" => "mdedsc","label" => "Área Cultural","type" => "string"));
            break;
            case "needsc":
                array_push($agp['agrupador'], array("campo" => "needsc","label" => "Segmento Cultural","type" => "string"));
            break;
            case "alodsc":
                array_push($agp['agrupador'], array("campo" => "alodsc","label" => "Alterações","type" => "string"));
            break;
            case "benparecertecnico":
                array_push($agp['agrupador'], array("campo" => "benparecertecnico","label" => "Parecer Técnico","type" => "string"));
            break;
            case "benparecerjuridico":
                array_push($agp['agrupador'], array("campo" => "benparecerjuridico","label" => "Parecer Jurídico","type" => "string"));
            break;
            case "benimpedimento":
                array_push($agp['agrupador'], array("campo" => "benimpedimento","label" => "Impedimento","type" => "string"));
            break;
            case "impdsc":
                array_push($agp['agrupador'], array("campo" => "impdsc","label" => "Motivo","type" => "string"));
            break;
            case "situacao_proposta":
                array_push($agp['agrupador'], array("campo" => "situacao_proposta","label" => "Situação Proposta","type" => "string"));
            break;
            case "esddsc":
                array_push($agp['agrupador'], array("campo" => "esddsc","label" => "Situação","type" => "string"));
            break;
        }
    }
    return $agp;
}

function monta_col(){
    $coluna = $_REQUEST['colunaNovo'] ? $_REQUEST['colunaNovo'] : $_REQUEST['coluna'];
    $coluna = ($coluna ? $coluna : array());
    $col = array();    
    foreach ( $coluna as $val ){
        switch( $val ){
            case "emeimpositiva":
                array_push($col,array("campo" => "emeimpositiva","label" => "Impositiva","type"	=> "string"));
            break;
            case "emejustificativa":
                array_push($col, array("campo" => "emejustificativa","label" => "Justificativa","type"=> "string"));
            break;
            case "emdvalorcusteio":
                array_push($col, array("campo" => "emdvalorcusteio","label" => "Valor Custeio","type"=> "numeric"));
            break;
            case "emdvalorcapital":
                array_push($col, array("campo" => "emdvalorcapital","label" => "Valor Capital","type"=> "numeric"));
            break;
            case "procnpj":
                array_push($col,array("campo" => "procnpj","label" => "CNPJ","type"	=> "string"));
            break;
            case "pronome":
                array_push($col,array("campo" => "pronome","label" => "Proponente","type"	=> "string"));
            break;
            case "esfdsc":
                array_push($col,array("campo" => "esfdsc","label" => "Localização","type"	=> "string"));
            break;
            case "paidescricao":
                array_push($col,array("campo" => "paidescricao","label" => "Paises","type"	=> "string"));
            break;
            case "estuf":
                array_push($col,array("campo" => "estuf","label" => "UFs","type"	=> "string"));
            break;
            case "mundescricao":
                array_push($col,array("campo" => "mundescricao","label" => "Municipios","type"	=> "string"));
            break;
            case "bennumeroprocesso":
                array_push($col,array("campo" => "bennumeroprocesso","label" => "Número do Processo","type"	=> "string"));
            break;
            case "beninicio":
                array_push($col,array("campo" => "beninicio","label" => "Data de Início da Vigência","type"	=> "string"));
            break;
            case "capdsc":
                array_push($col,array("campo" => "capdsc","label" => "Modalidade de Pactuação","type"	=> "string"));
            break;
            case "bented":
                array_push($col,array("campo" => "bented","label" => "TED","type"	=> "string"));
            break;
            case "benmotivoimpedimento":
                array_push($col,array("campo" => "benmotivoimpedimento","label" => "Justificativa","type"	=> "string"));
            break;
            case "bedvalorcusteio":
                array_push($col,array("campo" => "bedvalorcusteio","label" => "Vl. Beneficiário Custeio","type"	=> "numeric"));
            break;
            case "bedvalorcapital":
                array_push($col,array("campo" => "bedvalorcapital","label" => "Vl. Beneficiário Capital","type"	=> "numeric"));
            break;
            case "bedvalorpriorizadocusteio":
                array_push($col,array("campo" => "bedvalorpriorizadocusteio","label" => "Vl. Priorizado Custeio","type"	=> "numeric"));
            break;
            case "bedvalorpriorizadocapital":
                array_push($col,array("campo" => "bedvalorpriorizadocapital","label" => "Vl. Priorizado Capital","type"	=> "numeric"));
            break;
            case "contigenciadocusteio":
                array_push($col,array("campo" => "contigenciadocusteio","label" => "Vl. Contingenciado Custeio","type"	=> "numeric"));
            break;
            case "contigenciadocapital":
                array_push($col,array("campo" => "contigenciadocapital","label" => "Vl. Contingenciado Capital","type"	=> "numeric"));
            break;
            case "numero_programa":
                array_push($col,array("campo" => "numero_programa","label" => "Número Programa - SICONV","type"	=> "string"));
            break;
            case "titulo_programa":
                array_push($col,array("campo" => "titulo_programa","label" => "Título Programa - SICONV","type"	=> "string"));
            break;
            case "objeto_programa":
                array_push($col,array("campo" => "objeto_programa","label" => "Objeto Programa","type"	=> "string"));
            break;
            case "numero_proposta":
                array_push($col,array("campo" => "numero_proposta","label" => "Número Proposta - SICONV","type"	=> "string"));
            break;
            case "titulo_proposta":
                array_push($col,array("campo" => "titulo_proposta","label" => "Título Proposta - SICONV","type"	=> "string"));
            break;
            case "objeto_proposta":
                array_push($col,array("campo" => "objeto_proposta","label" => "Objeto Proposta","type"	=> "string"));
            break;        
            case "pliid":
                array_push($col,array("campo" => "pliid","label" => "Id PI","type"	=> "string"));
            break;        
            case "plicod":
                array_push($col,array("campo" => "plicod","label" => "Código PI","type"	=> "string"));
            break;        
            case "empenhado":
                array_push($col,array("campo" => "empenhado","label" => "Empenhado","type"	=> "numeric"));
            break;        
            case "pago":
                array_push($col,array("campo" => "pago","label" => "Pago","type"	=> "numeric"));
            break;        
        }
    }
    return $col;
}

function monta_join(){
    $join = array();
    
    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
    $agrupador = ($agrupador ? $agrupador : array());

    foreach ( $agrupador as $val ){
        switch( $val ){
            case "emeid":
                $novoJoin = "inner join public.unidadeorcamentaria uo on eme.unoid = uo.unoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "autcod":
                $novoJoin = "LEFT JOIN emendas.autor aut ON(aut.autid = eme.autid AND aut.autstatus = 'A')";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "autnome":
                $novoJoin = "LEFT JOIN emendas.autor aut ON(aut.autid = eme.autid AND aut.autstatus = 'A')";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "autsexo":
                $novoJoin = "LEFT JOIN emendas.autor aut ON(aut.autid = eme.autid AND aut.autstatus = 'A')";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "atpdsc":
                $novoJoin = "LEFT JOIN emendas.autor aut ON(aut.autid = eme.autid AND aut.autstatus = 'A')";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.autortipo autip ON aut.atpid = autip.atpid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "estuf":
                $novoJoin = "LEFT JOIN emendas.autor aut ON(aut.autid = eme.autid AND aut.autstatus = 'A')";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "parsigla":
                $novoJoin = "LEFT JOIN emendas.autor aut ON(aut.autid = eme.autid AND aut.autstatus = 'A')";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.partido p ON aut.parid = p.parid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "unocod":
                $novoJoin = "LEFT JOIN public.unidadeorcamentaria uno ON uno.unoid = eme.unoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
            break;
            case "unosigla":
                $novoJoin = "LEFT JOIN public.unidadeorcamentaria uno ON uno.unoid = eme.unoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "funcional":
                $novoJoin = "LEFT JOIN monitora.vw_ptres ptr ON ptr.ptrid = eme.ptrid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "benid":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "suocod":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN public.vw_subunidadeorcamentaria suo ON ben.suoid = suo.suoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "suonome":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN public.vw_subunidadeorcamentaria suo ON ben.suoid = suo.suoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "suocoddelegada":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.delegacao ed ON ben.benid = ed.benid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN public.vw_subunidadeorcamentaria suoed ON ed.suoid = suoed.suoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
            break;
            case "suonomedelegada":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.delegacao ed ON ben.benid = ed.benid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN public.vw_subunidadeorcamentaria suoed ON ed.suoid = suoed.suoid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
            break;
            case "mdedsc":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN monitora.pi_modalidadeensino mde ON ben.mdeid = mde.mdeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "needsc":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN monitora.pi_niveletapaensino nee ON ben.neeid = nee.neeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "alodsc":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "benparecertecnico":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "benparecerjuridico":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "benimpedimento":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "impdsc":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.impedimento imp ON ben.impid = imp.impid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
            break;
            case "situacao_proposta":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvbeneficiario sib ON(ben.benid = sib.benid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconv bensic on bensic.sicid = sib.sicid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvsituacao bensit on bensic.sitid = bensit.sitid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
            break;
            case "esddsc":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN workflow.documento doc ON ben.docid = doc.docid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
                $novoJoin = "LEFT JOIN workflow.estadodocumento doce ON doc.esdid = doce.esdid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                                
            break;
        }
    }  
    $coluna = $_REQUEST['colunaNovo'] ? $_REQUEST['colunaNovo'] : $_REQUEST['coluna'];
    foreach ( $coluna as $val ){
        switch( $val ){
            case "procnpj":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
                $novoJoin = "LEFT JOIN emendas.proponente pro ON pro.proid = ben.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
            break;
            case "pronome":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
                $novoJoin = "LEFT JOIN emendas.proponente pro ON pro.proid = ben.proid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "esfdsc":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN territorios.esfera esf ON ben.esfid = esf.esfid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }                
            break;
            case "paidescricao":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "estuf":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "mundescricao":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "bennumeroprocesso":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "beninicio":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "capdsc":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN monitora.pi_categoriaapropriacao pca ON ben.capid = pca.capid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "bented":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "benmotivoimpedimento":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "bedvalorcusteio":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "bedvalorcapital":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "bedvalorpriorizadocusteio":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "bedvalorpriorizadocapital":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "contigenciadocusteio":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "contigenciadocapital":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "numero_programa":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvbeneficiario sib ON(ben.benid = sib.benid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvprograma sip ON(sib.sicid = sip.sicid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.programa prg on prg.prgid = sip.prgid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "titulo_programa":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvbeneficiario sib ON(ben.benid = sib.benid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvprograma sip ON(sib.sicid = sip.sicid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.programa prg on prg.prgid = sip.prgid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "objeto_programa":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "numero_proposta":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvbeneficiario sib ON(ben.benid = sib.benid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconv bensic on bensic.sicid = sib.sicid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "titulo_proposta":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvbeneficiario sib ON(ben.benid = sib.benid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconv bensic on bensic.sicid = sib.sicid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;
            case "objeto_proposta":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconvbeneficiario sib ON(ben.benid = sib.benid)";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN emendas.siconv bensic on bensic.sicid = sib.sicid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;        
            case "pliid":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN monitora.vw_planointerno pi ON ben.pliid = pi.pliid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;        
            case "plicod":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN monitora.vw_planointerno pi ON ben.pliid = pi.pliid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;        
            case "empenhado":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN monitora.vw_planointerno pi ON ben.pliid = pi.pliid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;        
            case "pago":
                $novoJoin = "LEFT JOIN emendas.beneficiario ben ON ben.emeid = eme.emeid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
                $novoJoin = "LEFT JOIN monitora.vw_planointerno pi ON ben.pliid = pi.pliid";
                if (verificaJoin($join, $novoJoin)){
                    array_push($join,$novoJoin);
                }
            break;        
        }
    }    
    return $join;
}

function verificaJoin($join, $novoJoin){
    foreach($join as $key=> $value){
        if ($value==$novoJoin){
            return false;
        }
    }
    return true;
}

function monta_colunas(){
    $agrupador = $_REQUEST['agrupadorNovo'] ? $_REQUEST['agrupadorNovo'] : $_REQUEST['agrupador'];
    $agrupador = ($agrupador ? $agrupador : array());

    $colunas = array();

    foreach ( $agrupador as $val ){
        switch( $val ){
            case "emeid":
                array_push($colunas, "eme.emeid");
            break;
            case "emenumero":
                array_push($colunas, "SUBSTRING(eme.emenumero::TEXT FROM 1 FOR 4) || ' ' || SUBSTRING(eme.emenumero::TEXT FROM 5 FOR LENGTH(eme.emenumero::TEXT)) AS emenumero");
            break;
            case "autcod":
                array_push($colunas, "aut.autcod");
            break;
            case "autnome":
                array_push($colunas, "aut.autnome");
            break;
            case "autsexo":
                array_push($colunas, "case when aut.autsexo = 'M' then 'Homem' when aut.autsexo = 'F' then 'Mulher' else 'A Identificar' end genero");
            break;
            case "atpdsc":
                array_push($colunas, "autip.atpdsc");
            break;
            case "estuf":
                array_push($colunas, "aut.estuf");
            break;
            case "parsigla":
                array_push($colunas, "p.parsigla");
            break;
            case "unocod":
                array_push($colunas, "uno.unocod");
            break;
            case "unosigla":
                array_push($colunas, "uno.unosigla");
            break;
            case "funcional":
                array_push($colunas, "ptr.funcional || ' - ' || COALESCE(ptr.acatitulo, '') AS funcional");
            break;
            case "benid":
                array_push($colunas, "ben.benid");
            break;
            case "suocod":
                array_push($colunas, "suo.suocod");
            break;
            case "suonome":
                array_push($colunas, "suo.suonome");
            break;
            case "suocoddelegada":
                array_push($colunas, "suoed.suocod as suocoddelegada");
            break;
            case "suonomedelegada":
                array_push($colunas, "suoed.suonome as suonomedelegada");
            break;
            case "mdedsc":
                array_push($colunas, "mde.mdedsc");
            break;
            case "needsc":
                array_push($colunas, "nee.needsc");
            break;
            case "alodsc":
                array_push($colunas, "REPLACE(REPLACE(REPLACE(ARRAY(SELECT DISTINCT alo.alodsc FROM emendas.beneficiarioalteracao ba LEFT JOIN emendas.alteracaoorcamentaria alo ON ba.aloid = alo.aloid WHERE alo.alodsc IS NOT NULL AND ba.benid = ben.benid)::TEXT, '{', ''), '}', ''), '\"', '') AS alodsc");
            break;
            case "benparecertecnico":
                array_push($colunas, "CASE WHEN ben.benparecertecnico = 0 THEN 'Não' WHEN ben.benparecertecnico = 1 THEN 'Sim' WHEN ben.benparecertecnico = 2 THEN 'Não se aplica' END AS benparecertecnico");
            break;
            case "benparecerjuridico":
                array_push($colunas, "CASE WHEN ben.benparecerjuridico = 0 THEN 'Não' WHEN ben.benparecerjuridico = 1 THEN 'Sim' WHEN ben.benparecerjuridico = 2 THEN 'Não se aplica' END AS benparecerjuridico");
            break;
            case "benimpedimento":
                array_push($colunas, "CASE WHEN ben.benimpedimento IS NULL THEN 'Não' WHEN ben.benimpedimento = 0 THEN 'Não' WHEN ben.benimpedimento = 1 THEN 'Sim' WHEN ben.benimpedimento = 2 THEN 'Não se aplica' END AS benimpedimento");
            break;
            case "impdsc":
                array_push($colunas, "CASE WHEN ben.benimpedimento = 1 THEN imp.impdsc END as impdsc");
            break;
            case "situacao_proposta":
                array_push($colunas, "bensit.sitdsc as situacao_proposta");
            break;
            case "esddsc":
                array_push($colunas, "doce.esddsc");
            break;
        }
    }  
    $coluna = $_REQUEST['colunaNovo'] ? $_REQUEST['colunaNovo'] : $_REQUEST['coluna'];
    foreach ( $coluna as $val ){
        switch( $val ){
            case "emeimpositiva":
                array_push($colunas,"CASE WHEN eme.emeimpositiva = 't' THEN 'Sim' ELSE 'Não' END AS emeimpositiva");
            break;
            case "emejustificativa":
                array_push($colunas, "eme.emejustificativa");
            break;
            case "emdvalorcusteio":
                array_push($colunas, "coalesce((select sum(coalesce(emed.emdvalor,0)) from emendas.emendadetalhe emed where emed.emeid = eme.emeid and gndcod<=3),0) as emdvalorcusteio");
            break;
            case "emdvalorcapital":
                array_push($colunas, "coalesce((select sum(coalesce(emed.emdvalor,0)) from emendas.emendadetalhe emed where emed.emeid = eme.emeid and gndcod>3),0) as emdvalorcapital");
            break;
            case "procnpj":
                array_push($colunas, "REPLACE(TO_CHAR(pro.procnpj::NUMERIC, '00:000:000/0000-00'), ':', '.') as procnpj");
            break;
            case "pronome":
                array_push($colunas, "pro.pronome");
            break;
            case "esfdsc":
                array_push($colunas, "esf.esfdsc");
            break;
            case "paidescricao":
                array_push($colunas, "REPLACE(REPLACE(ARRAY(SELECT DISTINCT p.paidescricao FROM emendas.beneficiariolocalizacao bl JOIN territorios.pais p ON bl.paiid = p.paiid WHERE p.paidescricao IS NOT NULL AND bl.benid = ben.benid)::TEXT, '{', ''), '}', '') AS paidescricao");
            break;
            case "estuf":
                array_push($colunas, "REPLACE(REPLACE(ARRAY(SELECT DISTINCT bl.estuf FROM emendas.beneficiariolocalizacao bl WHERE bl.estuf IS NOT NULL AND bl.benid = ben.benid)::TEXT, '{', ''), '}', '') AS estuf");
            break;
            case "mundescricao":
                array_push($colunas, "REPLACE(REPLACE(ARRAY(SELECT DISTINCT m.mundescricao || '-' || m.estuf FROM emendas.beneficiariolocalizacao bl JOIN territorios.municipio m ON bl.muncod = m.muncod WHERE m.mundescricao IS NOT NULL AND bl.benid = ben.benid)::TEXT, '{', ''), '}', '') AS mundescricao");
            break;
            case "bennumeroprocesso":
                array_push($colunas, "ben.bennumeroprocesso");
            break;
            case "beninicio":
                array_push($colunas, "TO_CHAR(ben.beninicio,'dd/mm/YYYY') AS beninicio");
            break;
            case "capdsc":
                array_push($colunas, "pca.capdsc");
            break;
            case "bented":
                array_push($colunas, "CASE WHEN bented IS TRUE THEN 'Sim' ELSE 'Não' END AS bented");
            break;
            case "benmotivoimpedimento":
                array_push($colunas, "CASE WHEN ben.benimpedimento = 1 THEN benmotivoimpedimento END as benmotivoimpedimento");
            break;
            case "bedvalorcusteio":
                array_push($colunas, "coalesce((select sum(coalesce(case when eme.gndcod<=3 then bed.bedvalor else 0 end,0)) as bedvalor from emendas.beneficiariodetalhe bed left join emendas.emendadetalhe eme on bed.emdid = eme.emdid where bed.benid = ben.benid AND bed.bedstatus = 'A'),0) as bedvalorcusteio");
            break;
            case "bedvalorcapital":
                array_push($colunas, "coalesce((select sum(coalesce(case when eme.gndcod>3 then bed.bedvalor else 0 end,0)) as bedvalor from emendas.beneficiariodetalhe bed left join emendas.emendadetalhe eme on bed.emdid = eme.emdid where bed.benid = ben.benid AND bed.bedstatus = 'A'),0) as bedvalorcapital");
            break;
            case "bedvalorpriorizadocusteio":
                array_push($colunas, "(select sum(coalesce(case when eme.gndcod<=3 then bed.bedvalorpriorizado else 0 end,0)) from emendas.beneficiariodetalhe bed left join emendas.emendadetalhe eme on bed.emdid = eme.emdid where bed.benid = ben.benid AND bed.bedstatus = 'A') as bedvalorpriorizadocusteio");
            break;
            case "bedvalorpriorizadocapital":
                array_push($colunas, "(select sum(coalesce(case when eme.gndcod>3 then bed.bedvalorpriorizado else 0 end,0)) from emendas.beneficiariodetalhe bed left join emendas.emendadetalhe eme on bed.emdid = eme.emdid where bed.benid = ben.benid AND bed.bedstatus = 'A') as bedvalorpriorizadocapital");
            break;
            case "contigenciadocusteio":
                array_push($colunas, "coalesce((select sum(case when eme.gndcod<=3 then coalesce(bed.bedvalor,0) - coalesce(bed.bedvalorpriorizado,0) else 0 end) from emendas.beneficiariodetalhe bed left join emendas.emendadetalhe eme on bed.emdid = eme.emdid where bed.benid = ben.benid AND bed.bedstatus = 'A'),0) as contigenciadocusteio");
            break;
            case "contigenciadocapital":
                array_push($colunas, "coalesce((select sum(case when eme.gndcod>3 then coalesce(bed.bedvalor,0) - coalesce(bed.bedvalorpriorizado,0) else 0 end) from emendas.beneficiariodetalhe bed left join emendas.emendadetalhe eme on bed.emdid = eme.emdid where bed.benid = ben.benid AND bed.bedstatus = 'A'),0) as contigenciadocapital");
            break;
            case "numero_programa":
                array_push($colunas, "CASE WHEN prg.prgcodigo IS NOT NULL THEN substr(prg.prgcodigo::TEXT, 1, 6) || ' ' || substr(prg.prgcodigo::TEXT, 6, 4) || ' ' || substr(prg.prgcodigo::TEXT, 10, 4) ELSE NULL END AS numero_programa");
            break;
            case "titulo_programa":
                array_push($colunas, "prg.prgtitulo AS titulo_programa");
            break;
            case "objeto_programa":
                array_push($colunas, "ben.benprogramatitulo AS objeto_programa");
            break;
            case "numero_proposta":
                array_push($colunas, "CASE WHEN bensic.sicsequencial IS NOT NULL THEN bensic.sicsequencial || '/' || bensic.prsano END AS numero_proposta");
            break;
            case "titulo_proposta":
                array_push($colunas, "bensic.sicjustificativa AS titulo_proposta");
            break;
            case "objeto_proposta":
                array_push($colunas, "bensic.sicobjeto AS objeto_proposta");
            break;        
            case "pliid":
                array_push($colunas, "pi.pliid");
            break;        
            case "plicod":
                array_push($colunas, "pi.plicod");
            break;        
            case "empenhado":
                array_push($colunas, "pi.empenhado");
            break;        
            case "pago":
                array_push($colunas, "pi.pago");
            break;        
        }
    }
    return $colunas;
}
?>