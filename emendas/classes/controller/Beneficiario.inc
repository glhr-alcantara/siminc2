<?php

class Emendas_Controller_Beneficiario
{
    public function salvar($dados)
    {
        $url = '?modulo=principal/beneficiario_form&acao=A';

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($dados['benid']);
            $mBeneficiario->popularDadosObjeto($dados);

            $mBeneficiario->benempenhado = $dados['benempenhado'] == 't' ? 't' : 'f';
            $mBeneficiario->benconveniado = $dados['benconveniado'] == 't' ? 't' : 'f';
            $mBeneficiario->benpago = $dados['benpago'] == 't' ? 't' : 'f';
            $mBeneficiario->bented = $dados['bented'] == 't' ? 't' : 'f';

            $aCamposNulos = [
                # Cadastro Inicial
                'suoid', 'proid', 'esfid', 'benformaexecucao', 'bennumeroprocesso', 'beninicio', 'benpprogramanumero', 'benpropostanumero', 'benpropostatitulo', 'benpropostaobjeto', 'impid', 'mdeid', 'neeid', 'capid', 'bented',
                # Cadastro Complementar pra o PI
                'plititulo', 'plidsc', 'oppid', 'mppid', 'ippid', 'mpnid', 'ipnid', 'pprid', 'pumid', 'picquantidade'
            ];
            $mBeneficiario->salvar(null, null, $aCamposNulos);

            if($dados['req'] == 'salvar-complemento'){
                $mBeneficiario->associarSniic($dados);
                $mBeneficiario->associarPronac($dados);
                $mBeneficiario->associarAnexo($dados);
            } else {
                $mBeneficiario->associarDelegacao($dados);
                $mBeneficiario->associarAlteracao($dados);

                // Salvando dados da Localização
                $dados['benid'] = $mBeneficiario->benid;
                (new Emendas_Controller_BeneficiarioLocalizacao())->salvar($dados);
            }

            $mBeneficiario->commit();
            simec_redirecionar($url. '&benid='. $mBeneficiario->benid. ($dados['req'] == 'salvar-complemento'? '&aba=1': NULL), 'success');
        } catch (Exception $e){
            $mBeneficiario->rollback();
            simec_redirecionar($url. ($dados['req'] == 'salvar-complemento'? '&aba=1': NULL), 'error');
        }
    } //end salvar()

    public function excluir($benid)
    {
        $url = '?modulo=principal/beneficiario_form&acao=A&benid='. (int)$benid;

        try {
            $mBeneficiario = new Emendas_Model_Beneficiario($benid);

            $url = '?modulo=principal/emenda_form&acao=A&emeid='. (int)$mBeneficiario->emeid;

            $mBeneficiario->benstatus = 'I';
            $mBeneficiario->salvar();
            $mBeneficiario->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mBeneficiario->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()

    public function montarFormularioHistorico($benid = null, $behid = null)
    {
        global $simec;
        $mBeneficiarioHistorico = new Emendas_Model_BeneficiarioHistorico($behid);
        if(!$mBeneficiarioHistorico->benid){ $mBeneficiarioHistorico->benid = $benid; }
        $mBeneficiarioHistorico->behdata = $mBeneficiarioHistorico->behdata ? $mBeneficiarioHistorico->behdata : date('d/m/Y');
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariohistorico_form.inc';
    }

    public function montarListagemHistorico($benid, $podeEditar)
    {
        $aBeneficiarioHistorico = (new Emendas_Model_BeneficiarioHistorico)->recuperarPorUnidade($benid);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariohistorico.inc';
    }

    public function montarFormularioDetalhe($emeid, $benid = null, $bedid = null)
    {
        global $simec;
        $mBeneficiarioDetalhe = new Emendas_Model_BeneficiarioDetalhe($bedid);
        $emeid = $emeid;
        if(!$mBeneficiarioDetalhe->benid){
            $mBeneficiarioDetalhe->benid = $benid;
        }
        $mBeneficiario = new Emendas_Model_Beneficiario($mBeneficiarioDetalhe->benid);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariodetalhe_form.inc';
    }

    public function montarListagemDetalhe($benid, $podeEditar)
    {
        $aBeneficiarioDetalhe = (new Emendas_Model_BeneficiarioDetalhe)->recuperarListagem($benid);
        include_once APPRAIZ . 'emendas/modulos/principal/beneficiariodetalhe.inc';
    }

    public function montarListagemSiconv($benid)
    {
        $aSiconv = (new Emendas_Model_Siconv)->recuperarListagem($benid);
        include_once APPRAIZ . 'emendas/modulos/principal/siconv.inc';
    }

    public function montarDetalheSiconv($sicid)
    {
        $aSiconv = (new Emendas_Model_Beneficiario())->recuperarDadosSiconv($sicid);
        include_once APPRAIZ . 'emendas/modulos/principal/siconv-detalhe.inc';
    }

    public function atualizarSiconv($emeid, $benid)
    {
        $mEmenda = new Emendas_Model_Emenda($emeid);
        $mEmenda->atualizarSiconv($benid);
        simec_redirecionar('?modulo=principal/beneficiario_form&acao=A&benid=7966&benid='. $benid, 'success');
    }

    public function atualizarPrograma($emeid)
    {
        $mEmenda = new Emendas_Model_Emenda($emeid);
        $mEmenda->atualizarPrograma();
        ver($benid, $mEmenda, d);
    }
}
