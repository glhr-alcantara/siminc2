<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' . '.xls"' );

    $cabecalho = [
        'ID Emenda',
        'N�mero',
        'Cod. Autor',
        'Autor',
        'G�nero',
        'Tipo Parlamentar',
        'UF Parlamentar',
        'Partido',
        'Cod. Unidade',
        'Unidade',
        'Funcional',
        'Impositiva',
        'Justificativa',
        # DADOS FINANCEIROS
        /*'GND',
        'Mod. Aplica��o',
        'Fonte',*/
        'Valor Custeio',
        'Valor Capital',
        # BENEFICIARIOS
        'Id Beneficiario',
        'C�digo Subunidade',
        'Subunidade',
        'C�digo Subunidade delegada',
        'Subunidade delegada',
        'CNPJ',
        'Proponente',
        'Localiza��o',
        'Paises',
        'UFs',
        'Municipios',
        'N�mero do Processo',
        'Data de In�cio da Vig�ncia',
        'Modalidade de Pactua��o',
        'TED',
        '�rea Cultural',
        'Segmento Cultural',
        'Altera��es',
        'Parecer T�cnico',
        'Parecer Jur�dico',
        'Impedimento',
        'Motivo',
        'Justivicativa',
        # DADOS FINANCEIROS DO BENFICI�RIO
        'Vl. Benefici�rio Custeio',
        'Vl. Benefici�rio Capital',
        'Vl. Priorizado Custeio',
        'Vl. Priorizado Capital',
        'Vl. Contingenciado Custeio',
        'Vl. Contingenciado Capital',
        # DADOS SICONV
        'N�mero Programa - SICONV',
        'T�tulo Programa - SICONV',
        'Objeto Programa',
        'N�mero Proposta - SICONV',
        'T�tulo Proposta - SICONV',
        'Objeto Proposta',
        'Situa��o Proposta',
        # SITUA��O WORKFLOW
        'Situa��o',
        # PLANO INTERNO
        'Id PI',
        'C�digo PI',
        'Empenhado',
        'Pago',
    ];

    # Lista de colunas que ter�o formata��o de moeda.
    $listaColunaMoeda = array(
        'emdvalor',
        'bedvalor',
        'bedvalorpriorizado',
        'contigenciado',
        'empenhado',
        'pago'
    );

    $sql = (new Emendas_Model_Emenda)->montarSqlRelatorioGeral($_SESSION['exercicio'],$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf'])));
//ver($sql);
    $lista = $db->carregar($sql);
//ver($lista, d);
?>

<table width="100%" cellspacing="0">
    <thead>
        <tr>
            <?php foreach($cabecalho as $coluna){ ?>
                <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4"><?php echo $coluna; ?></th>
            <?php } ?>
        </tr>
    </thead>
    <tbody>
        <?php foreach($lista as $dado){ ?>
            <tr>
                <?php foreach($dado as $coluna => $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    </tbody>
</table>

<?php
die;
