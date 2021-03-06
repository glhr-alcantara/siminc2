<?php

$listaNaturezas = (new Public_Model_NaturezaDespesa())->recuperarTodos('*', ["ndpstatus = 'A'", "ndpano = '". $_SESSION['exercicio']. "'"], 'ndpdsc');

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

                    <a class="btn btn-small btn-warning" href="?modulo=apoio/natureza_despesa_form&acao=A">Novo</a>

                    <div class="table-responsive">
                        <table class="table table-striped table-bordered table-hover dataTables">
                            <thead>
                            <tr class="text-center">
                                <th style="10%">A��es</th>
                                <th>C�digo Da Natureza</th>
                                <th>Descri��o</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            if($listaNaturezas){
                                foreach($listaNaturezas as $dados){
                                    ?>
                                    <tr>
                                        <td class="text-center">
                                            <a title="Alterar" href="?modulo=apoio/natureza_despesa_form&acao=A&ndpid=<?= $dados['ndpid']; ?>"><i class="fa fa-pencil"></i></a>
                                        </td>
                                        <td><?= $dados['ndpcod']?></td>
                                        <td><?= $dados['ndpdsc']?></td>
                                    </tr>
                                <?php } } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
