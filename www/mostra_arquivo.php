<?php
    // carrega as fun��es gerais
    require_once "config.inc";
    require_once APPRAIZ . "includes/funcoes.inc";
    require_once APPRAIZ . "includes/classes_simec.inc";
    require_once APPRAIZ . "includes/arquivo.inc";

    # Verifica se a sess�o n�o expirou, se tiver expirada envia pra tela de login.
    controlarAcessoSemAutenticacao();
    
    $db = new cls_banco();
    $intIdArquivo = (integer) $_REQUEST["id"];
    mostraArquivo($intIdArquivo);
?>