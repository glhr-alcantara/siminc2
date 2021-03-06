<?php

// $aObjetivoppa = (new Public_Model_ObjetivoPpa())->recuperarTodos('*', ["oppstatus = 'A'","prsano = '{$_SESSION['exercicio']}'"], 'oppdsc');
$sqlObjetivo = (new Public_Model_ObjetivoPpa())->recuperarSqlCombo();
$aObjetivoppa = $db->carregar($sqlObjetivo);

include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">

                    <a class="btn btn-small btn-warning" href="?modulo=apoio/objetivoppa_form&acao=A">Novo</a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables" >
                            <thead>
                            <tr class="text-center">
                                <th width="10%">A��es</th>
                                <th>Descri��o</th>
                            </tr>
                            </thead>
                            <tbody>
                              <?php foreach($aObjetivoppa as $dados){ ?>

                                <tr>
                                    <td class="text-center">
                                        <a title="Alterar" href="planacomorc.php?modulo=apoio/objetivoppa_form&acao=A&oppid=<?php echo $dados['codigo']; ?>"><i class="fa fa-pencil"></i></a>
                                    </td>
                                    <td><?php echo $dados['descricao']; ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
