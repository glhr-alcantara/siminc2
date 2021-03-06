<form id="formulario" name="formulario" method="post" class="form-horizontal">
    <input type="hidden" name="req" id="req" value="salvar-medida" />
    <input name="suoid" id="suoid" type="hidden" value="<?php echo $mSubUnidade->suoid;?>">
    <input name="unoid" id="unoid" type="hidden" value="<?php echo $mSubUnidade->unoid; ?>">

    <?php
    echo $simec->input('suocod', 'C�digo', $mSubUnidade->suocod, ['required', 'class'=>"inteiro", 'maxlength' => 6]);
    echo $simec->input('suonome', 'Nome', $mSubUnidade->suonome, ['required', 'maxlength' => 200]);
    echo $simec->input('suosigla', 'Sigla', $mSubUnidade->suosigla, ['required', 'maxlength' => 20]);
    echo $simec->radio('suoflaglimiteliberado', 'Limite Liberado', $mSubUnidade->suoflaglimiteliberado, ['t'=>'Sim', 'f'=>'N�o']);
    echo $simec->input('suocodigopi', 'C�digo PI', $mSubUnidade->suocodigopi, ['required', 'maxlength' => 2]);
    ?>

    <div class="form-group">
        <div class="text-center">
            <button class="btn btn-primary" id="#btn-salvar-medida" type="submit"><i class="fa fa-check"></i>&nbsp;Salvar</button>
        </div>
    </div>
</form>

<script type="text/javascript">
    $(function(){

        $('#btn-salvar-medida').click(function(){
            options = {
                success: function() {
                    // Carregar a listagem
                    jQuery("#div_listagem_medida").load('?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=listar-medida&$unoid='+$('#$unoid').val());

                    // Zerar o formul�rio
                    jQuery("#div_formulario_medida").load('?modulo=apoio/unidadeOrcamentaria_form&acao=A&req=formulario-medida&$unoid='+$('#$unoid').val());
                }
            }

            jQuery("#formulario-medida").ajaxForm(options).submit();
            return false;
        });

    });
</script>
