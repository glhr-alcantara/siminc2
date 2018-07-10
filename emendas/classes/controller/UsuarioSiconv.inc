<?php

class Emendas_Controller_UsuarioSiconv
{
    public function salvar($dados)
    {
        $url = '?modulo=configuracao/usuario_siconv_form&acao=A';

        try {
            $mUsuarioSiconv = new Emendas_Model_UsuarioSiconv($dados['ussid']);
            $mUsuarioSiconv->popularDadosObjeto($dados);
            $mUsuarioSiconv->usucpf = str_replace('.', '', str_replace('-', '', $mUsuarioSiconv->usucpf));
            $mUsuarioSiconv->ususenhasiconv = base64_encode($mUsuarioSiconv->ususenhasiconv);
            $mUsuarioSiconv->salvar(null, null, []);
            $mUsuarioSiconv->commit();
            simec_redirecionar($url, 'success');
        } catch (Exception $e){
            $mUsuarioSiconv->rollback();
            simec_redirecionar($url, 'error');
        }
    } //end salvar()
    
}            