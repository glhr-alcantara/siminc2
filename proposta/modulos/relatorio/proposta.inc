<?php

    header( 'Content-Type: application/vnd.ms-excel' );
    header( 'Expires: 0' );
    header( 'Cache-Control: must-revalidate, post-check=0, pre-check=0' );
    header( 'Content-Disposition: attachment; filename="relatorio_geral_pi' . '.xls"' );

    $cabecalho = [
        'ID Proposta',
        'Subunidade',
        'Enquadramento da Despesa',
        'RP',
        'Funcional',
        'A��o',
        'PO',
        'Quantidade Localizador',
        'Quantidade PO',
        'Quantidade Expans�o PO',
        'Justificativa',
        'Justificativa Expans�o',
        'Nat. Desp.',
        'IDUSO',
        'Fonte',
        'IDOC',
        'Valor',
        'Valor Expans�o',
    ];

    # Lista de colunas que ter�o formata��o de moeda.
    $listaColunaMoeda = array(
        'prdvalor',
        'prdvalorexpansao'
    );

    $sql = montarSqlRelatorioGeralProposta((object) array('exercicio' => $_SESSION['exercicio']));
//ver($sql, d);
    $lista = $db->carregar($sql);
?>

<table width="100%" cellspacing="0">
    <thead>
        <tr>
            <?php foreach($cabecalho as $coluna){ ?>
                <th style="border: 1px #e5d9c5 solid; background-color: #f4f4f4"><?php echo $coluna; ?></th>
            <?php } ?>
        </tr>
    </thead>
    <?php if($lista): ?>
    <tbody>
        <?php foreach($lista as $dado){ ?>
            <tr>
                <?php foreach($dado as $coluna => $valor){ ?>
                    <td style="border: 1px #e5d9c5 solid;"><?php echo in_array($coluna, $listaColunaMoeda)? formata_valor($valor): $valor; ?></td>
                <?php } ?>
            </tr>
        <?php } ?>
    </tbody>
    <?php endif; ?>
</table>

<?php
die;
