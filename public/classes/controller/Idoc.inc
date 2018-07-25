<?php

class Public_Controller_Idoc
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/idoc_form&acao=A';

        try {
            $mdidoc = new Public_Model_Idoc($dados['idoid']);
            $mdidoc->popularDadosObjeto($dados);
//            $mdidoc->ndpano = $mdidoc->ndpano ? $mdidoc->ndpano : $_SESSION['exercicio'];
//            ver($mdnatDespesa,d);
            $mdidoc->salvar(null,null);
            $mdidoc->commit();
            simec_redirecionar($url . '&idoid=' . $mdidoc->idoid, 'success');
        } catch (Exception $e){
            $mdidoc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($idoid)
    {
        $url = '?modulo=apoio/idoc&acao=A';

        try {
            $mdidoc = new Public_Model_Idoc($idoid);
            $mdidoc->idostatus = 'I';
            $mdidoc->salvar();
            $mdidoc->commit();
            simec_redirecionar($url . '&idoid=' . $mdidoc->idoid, 'success');
        } catch (Exception $e){
            $mdidoc->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
