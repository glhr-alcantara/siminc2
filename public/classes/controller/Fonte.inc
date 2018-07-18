<?php

class Public_Controller_Fonte
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/fonte_form&acao=A';

        try {
            $mdfonte = new Public_Model_FonteRecurso($dados['foncod']);
            $mdfonte->popularDadosObjeto($dados);
            $mdfonte->salvar(null,null);
            $mdfonte->commit();
            simec_redirecionar($url . '&foncod=' . $mdfonte->foncod, 'success');
        } catch (Exception $e){
            $mdfonte->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($foncod)
    {
        $url = '?modulo=apoio/fonte&acao=A';
        try {
            $mdfonte = new Public_Model_FonteRecurso($foncod);
            $mdfonte->fonstatus = 'I';
            $mdfonte->salvar();
            $mdfonte->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mdfonte->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
