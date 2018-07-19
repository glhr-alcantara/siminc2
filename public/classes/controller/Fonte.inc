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


    /**
     * Valida se o código da Fonte pode ser alterado ou inserido.
     *
     * @param array $request
     * @return array
     */
    function validaFonte($request)
    {
        $resposta = array();
        # Verifica se o código inserido está sendo usado.
        $mFonte = new Public_Model_FonteRecurso($request['foncod']);
        $mFonte->carregarPorCodigo();
//ver($mFonte->foncod,d);
        if (!empty($mFonte->foncod)) {
            $resposta['title'] = NULL;
            $resposta['text'] = 'O código da Fonte ' . $mFonte->foncod . ' já está sendo usado!';
            $resposta['type'] = 'error';
        } else {
            $resposta['type'] = 'success';
        }

        return simec_json_encode($resposta);
    }
}
