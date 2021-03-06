<?php

    $cAcompanhamento = new Acompanhamento_Controller_Acompanhamento();
    switch ($_REQUEST['req']) {
        case 'salvar':
            $cAcompanhamento->salvar($_REQUEST, $urlModulo);
        die;
        case 'excluir':
            $cAcompanhamento->excluir($_REQUEST['acoid']);
        die;
        case 'ver-historico':
            $cAcompanhamento->recuperarHistorico($_REQUEST['acoid']);
        die;
        case 'carregarMedida':
            $mClassificacao = new Acompanhamento_Model_Classificacao($_REQUEST['claid']);
            $retorno['medida'] = $mClassificacao->clamedida;
            $retorno['select'] = $simec->select('medidas[]', 'Medidas a serem adotadas', null, (new Acompanhamento_Model_Medida())->recuperarSqlCombo(null, ['claid = ' . (int)$_REQUEST['claid']]));

            echo json_encode($retorno);
        die;
        case 'uploadAnexoAcompanhamento':
            $file = new FilesSimec();
            $file->setEsquema('acompanhamento');
            if($file->setUpload($_REQUEST['arqdescricao'], '', false)){
                echo simec_json_encode(array(
                    'arqid' => $file->getIdArquivo(),
                    'arqnome' => $_FILES['file']['name'],
                    'arqdescricao' => $_REQUEST['arqdescricao']
                ));
            } else {
                echo simec_json_encode(array(
                    'error' => 1,
                    'errorMensage' => 'N�o foi poss�vel enviar o arquivo!'
                ));
            }
        die;
        case 'historico-pi':
            include_once APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_modal.inc";
        die;
        case 'historico-pi-usuario':
            include_once APPRAIZ. "planacomorc/modulos/principal/unidade/historico_pi_usuario_modal.inc";
        die;
    }

