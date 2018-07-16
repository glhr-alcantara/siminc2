<?php

class Spo_Controller_SubUnidadeIniciativaPpa
{
    public function salvar($dados)
    {
        $url = 'planacomorc.php?modulo=apoio/vincular-iniciativa-ppa&acao=A';

        try {
            $oModel = new Spo_Model_SubUnidadeIniciativaPpa();
            $oModel->excluirPorExercicio($_SESSION['exercicio']);

            if(isset($dados['vinculos']) && is_array($dados['vinculos'])){
                foreach($dados['vinculos'] as $ippid => $vinculos){
                    foreach($vinculos as $suoid){
                        $oModel->ippid = $ippid;
                        $oModel->suoid = $suoid;
                        $oModel->salvar();
                        $oModel->suoippid = null;
                    }
                }
            }
            $oModel->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $prefeitura->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}