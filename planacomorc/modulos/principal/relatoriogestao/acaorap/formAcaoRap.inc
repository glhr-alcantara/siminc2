<?php
/**
 * @version $Id: formAcaoRap.inc 93291 2015-01-29 19:04:04Z lindalbertofilho $
 */

$ano = $_SESSION['exercicio'] - 1;
echo $fm->getMensagens();
?>
<form name="relGestao" id="relGestao" method="POST" role="form" class="form-horizontal">
    <input name="dados[rldid]" type="hidden" id="rldid" value="<?php echo $dados['rldid']; ?>" />
    <input name="dados[rldtipo]" type="hidden" value="<?php echo $abaAtiva; ?>" />
    <input name="dados[rldacaoprioritaria]" id="rldacaoprioritaria" type="hidden"
           value="<?php echo $dados['rldacaoprioritaria']; ?>" />
    <input name="dados[rldacaoprioritariatipo]" id="rldacaoprioritariatipo" type="hidden"
           value="<?php echo $dados['rldacaoprioritariatipo']; ?>" />
    <input name="requisicao" type="hidden" value="salvar" />

    <div class="bs-callout bs-callout-primary">
        <h4>Identifica��o da a��o</h4>
        <div class="form-group row">
            <label class="control-label col-md-2" for="acacod">C�digo:</label>
            <div class="col-md-10">
                <?php inputTexto('dadosacao[acacod]', $dadosacao['acacod'], 'acacod', 4, false, $configHabil); ?>
            </div>
        </div>
        <div class="form-group row">
            <label class="control-label col-md-2" for="rldtipocod">Tipo:</label>
            <div class="col-md-10">
                <?php inputTexto('dados[rldtipocod]', $dados['rldtipocod'], 'rldtipocod', 100, false); ?>
            </div>
        </div>
        <div class="form-group row">
            <label class="control-label col-md-2" for="rldtitulo">T�tulo:</label>
            <div class="col-md-10">
                <?php inputTexto('dados[rldtitulo]', $dados['rldtitulo'], 'rldtitulo', 200, false); ?>
            </div>
        </div>
        <div class="form-group row">
            <label class="control-label col-md-2" for="rldiniciativa">Iniciativa:</label>
            <div class="col-md-10">                
                <?php inputTextArea('dados[rldiniciativa]', $dados['rldiniciativa'], 'rldiniciativa', 4000); ?>
            </div>
        </div>
        <div class="bs-callout bs-callout-primary">
            <h4>Objetivo</h4>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldobjetivo">Descri��o:</label>
                <div class="col-md-10">                
                <?php inputTextArea('dados[rldobjetivo]', $dados['rldobjetivo'], 'rldobjetivo', 4000); ?>
                </div>
            </div>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldcodigoobjetivo">C�digo:</label>
                <div class="col-md-10">
                <?php inputTexto('dados[rldcodigoobjetivo]', $dados['rldcodigoobjetivo'], 'rldcodigoobjetivo', 200, false); ?>
                </div>
            </div>
        </div>
        <div class="bs-callout bs-callout-primary">
            <h4>Programa</h4>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldprograma">Descri��o:</label>
                <div class="col-md-10">
                <?php inputTexto('dados[rldprograma]', $dados['rldprograma'], 'rldprograma', 200, false); ?>
                </div>
            </div>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldcodigoprograma">C�digo:</label>
                <div class="col-md-10">
                <?php inputTexto('dados[rldcodigoprograma]', $dados['rldcodigoprograma'], 'rldcodigoprograma', 4, false); ?>
                </div>
            </div>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldtipoprograma">Tipo:</label>
                <div class="col-md-10">
                <?php inputTexto('dados[rldtipoprograma]', $dados['rldtipoprograma'], 'rldtipoprograma', 200, false); ?>
                </div>
            </div>
        </div>
        <div class="form-group row">
            <label class="control-label col-md-2" for="unicod">Unidade Or�ament�ria:</label>
            <div class="col-md-10">
                <?php inputTexto('dadosacao[unicod]', "{$dadosacao['unicod']} - {$dadosacao['unidsc']}", 'unicod', 5, false, $configHabil); ?>
            </div>
        </div>
        <div class="form-group row">
            <label class="control-label col-md-2"> A��o Priorit�ria:</label>
            <div class="col-md-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default">
                        <input type="radio" class="radio_acaprioritaria" id="acaoprioritaria_t" value="t" /> Sim
                    </label>
                    <label class="btn btn-default active">
                        <input type="radio" class="radio_acaprioritaria" id="acaoprioritaria_f" value="f" checked /> N�o
                    </label>
                </div>
            </div>
        </div>
        <div id="casoPositivo" style="display:none">
            <div class="form-group row">
                <label class="control-label col-md-2"> Caso Positivo:</label>
                <div class="col-md-10">
                    <div class="btn-group" data-toggle="buttons">
                        <label class="btn btn-default">
                            <input type="radio" class="radio_acaprioritipo" id="acaprioritariatipo_p" value="P" /> PAC
                        </label>
                        <label class="btn btn-default">
                            <input type="radio" class="radio_acaprioritipo" id="acaprioritariatipo_b" value="B" /> Brasil sem mis�ria
                        </label>
                        <label class="btn btn-default">
                            <input type="radio" class="radio_acaprioritipo" id="acaprioritariatipo_o" value="O" /> Outras
                        </label>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <div class="bs-callout bs-callout-primary">
        <h4>Restos a Pagar n�o processados - Exerc�cios anteriores</h4>
        <div class="bs-callout bs-callout-primary">
            <h4>Execu��o Or�ament�ria e Financeira</h4>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldrapeaem0101">Valor em 01/01/<?php echo $_SESSION['exercicio']; ?>:</label>
                <div class="col-md-10 input-group">
                    <div class="input-group-addon">R$</div>
                    <?php inputTexto('dados[rldrapeaem0101]', $dados['rldrapeaem0101'], 'rldrapeaem0101', 20, true, $configClasse); ?>
                </div>
            </div>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldrapeavalorliquidado">Valor Liquidado:</label>
                <div class="col-md-10 input-group">
                    <div class="input-group-addon">R$</div>
                    <?php inputTexto('dados[rldrapeavalorliquidado]', $dados['rldrapeavalorliquidado'], 'rldrapeavalorliquidado', 20, true, $configClasse); ?>
                </div>
            </div>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldrapeavalorcancelado">Valor Cancelado:</label>
                <div class="col-md-10 input-group">
                    <div class="input-group-addon">R$</div>
                    <?php inputTexto('dados[rldrapeavalorcancelado]', $dados['rldrapeavalorcancelado'], 'rldrapeavalorcancelado', 20, true, $configClasse); ?>
                </div>
            </div>
        </div>
        <div class="bs-callout bs-callout-primary">
            <h4>Execucao F�sica - Metas</h4>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldrapeadescricaometa"> Descri��o da Meta:</label>
                <div class="col-md-10">
                    <?php inputTexto('dados[rldrapeadescricaometa]', $dados['rldrapeadescricaometa'], 'rldrapeadescricaometa', 200, false); ?>
                </div>
            </div>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldrapeaunidademedida">Unidade de Medida:</label>
                <div class="col-md-10">
                    <?php inputTexto('dados[rldrapeaunidademedida]', $dados['rldrapeaunidademedida'], 'rldrapeaunidademedida', 200, false); ?>
                </div>
            </div>
            <div class="form-group row">
                <label class="control-label col-md-2" for="rldrapearealizado">Realizada:</label>
                <div class="col-md-10">
                    <?php inputTexto('dados[rldrapearealizado]', $dados['rldrapearealizado'], 'rldrapearealizado', 20, false, $configMasc); ?>
                </div>
            </div>
        </div>
    </div>
    <button type="submit" class="btn btn-primary" id="salvarAca"><span class="glyphicon glyphicon-ok"></span> Salvar A��o</button>
    <?php if (!empty($dados['rldid'])): ?>
    <button type="button" class="btn btn-success" id="gerar-pdf"><span class="glyphicon glyphicon-save"></span> Gerar PDF</button>
    <button type="button" class="btn btn-success" id="gerar-xls"><span class="glyphicon glyphicon-save"></span> Gerar XLS</button>
    <?php endif; ?>
</form>
