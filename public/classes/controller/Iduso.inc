<?php

class Public_Controller_Iduso
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/iduso_form&acao=A';

        try {
            $mdIduso = new Public_Model_IdentifUso($dados['iduid']);
            $mdIduso->popularDadosObjeto($dados);
            $mdIduso->salvar();
            $mdIduso->commit();
            simec_redirecionar($url . '&iduid=' . $mdIduso->iduid, 'success');
        } catch (Exception $e){
            $mdIduso->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
        
    public function excluir($iduid)
    {
        $url = '?modulo=apoio/iduso&acao=A';

        try {
            $mdIduso = new Public_Model_IdentifUso($iduid);
            $mdIduso->idustatus = 'I';
            $mdIduso->salvar();
            $mdIduso->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdIduso->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
