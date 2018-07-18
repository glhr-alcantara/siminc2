<?php

class Public_Controller_Iduso
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/iduso_form&acao=A';

        try {
            $mdIduso = new Public_Model_IdentifUso($dados['iducod']);
            $mdIduso->popularDadosObjeto($dados);
            $mdIduso->idustatus = 'A';
            $mdIduso->salvar();
            $mdIduso->commit();
            simec_redirecionar($url . '&iducod=' . $mdIduso->iducod, 'success');
        } catch (Exception $e){
            $mdIduso->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
        
    public function excluir($iducod)
    {
        $url = '?modulo=apoio/iduso&acao=A';

        try {
            $mdIduso = new Public_Model_IdentifUso($iducod);
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
