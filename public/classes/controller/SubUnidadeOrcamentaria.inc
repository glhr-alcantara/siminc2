<?php

class Public_Controller_SubUnidadeOrcamentaria
{
    public function salvar($dados)
    {
        $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&unoid=' . $dados['unoid'];
        try {
            $mUnidadeOrcamentaria = new Public_Model_SubUnidadeOrcamentaria($dados['suoid']);
            $mUnidadeOrcamentaria->popularDadosObjeto($dados);
            $mUnidadeOrcamentaria->salvar(null, null, ['unoid','suocod', 'suonome', 'suostatus','suoflaglimiteliberado','suocodigopi' ]);
            $mUnidadeOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function excluir($dados)
    {
      $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&unoid=' . $dados['unoid'];
        try {
            $mUnidadeOrcamentaria = new Public_Model_SubUnidadeOrcamentaria($dados['suoid']);

            if($mUnidadeOrcamentaria->existeVinculo()){
                $mensagem = 'O registro n�o pode ser exclu�do pois possui v�nculo com algum PI.';
                $url = '?modulo=apoio/unidadeOrcamentaria_form&acao=A&suoid=' . $dados['unoid'];
                simec_redirecionar($url, 'error', $mensagem);
            }

            $mUnidadeOrcamentaria->suostatus = 'I';
            $mUnidadeOrcamentaria->salvar();
            $mUnidadeOrcamentaria->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mUnidadeOrcamentaria->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
}
