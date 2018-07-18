<?php

class Public_Controller_NaturezaDespesa
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/natureza_despesa_form&acao=A';

        try {
            $mdnatDespesa = new Public_Model_NaturezaDespesa($dados['ndpid']);
            $mdnatDespesa->popularDadosObjeto($dados);
            $mdnatDespesa->ndpano = $mdnatDespesa->ndpano ? $mdnatDespesa->ndpano : $_SESSION['exercicio'];
//            ver($mdnatDespesa,d);
            $mdnatDespesa->salvar(null,null);
            $mdnatDespesa->commit();
            simec_redirecionar($url . '&ndpid=' . $mdnatDespesa->ndpid, 'success');
        } catch (Exception $e){
            $mdnatDespesa->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($ndpid)
    {
        $url = '?modulo=apoio/natureza_despesa&acao=A';

        try {
            $mdnatDespesa = new Public_Model_NaturezaDespesa($ndpid);
            $mdnatDespesa->ndpstatus = 'I';
            $mdnatDespesa->salvar();
            $mdnatDespesa->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdnatDespesa->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
