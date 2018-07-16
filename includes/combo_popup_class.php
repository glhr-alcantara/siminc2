<?php

class combo_popup{
    
    public $nome;
    public $sql; 
    public $titulo;
    public $tamanho_janela = '400x400';
    public $maximo_itens = 0;
    public $codigos_fixos = array();
    public $mensagem_fixo = ''; 
    public $habilitado = 'S';
    public $campo_busca_codigo = false;
    public $campo_flag_contem = false;
    public $size = 10;
    public $width = 400;
    public $onpop = null;
    public $onpush = null;
    public $param_conexao = false;
    public $where=null;
    public $value = null;
    public $mostraPesquisa = true;
    public $campo_busca_descricao = false;
    public $funcaoJS=null;
    public $intervalo=false;
    public $arrVisivel = null;
    public $arrOrdem = null;
    
    public function gerarComponente(){
        global ${$this->nome};
	unset($dados_sessao);
	// prepara parametros
	$maximo_itens = abs( (integer) $maximo_itens );
	$codigos_fixos = $codigos_fixos ? $codigos_fixos : array();
	// prepara sessão
	$dados_sessao = array(
	'sql' => (string) $this->sql, // o sql é armazenado para ser executado posteriormente pela janela popup
	'titulo' => $this->titulo,
	'indice' => $indice_visivel,
	'maximo' => $maximo_itens,
	'codigos_fixos' => $this->codigos_fixos,
	'mensagem_fixo' => $this->mensagem_fixo,
	'param_conexao' => $this->param_conexao,
	'where'	        => $this->where,
	'mostraPesquisa'=> $this->mostraPesquisa,
	'intervalo'     => $this->intervalo,
	'arrVisivel'    => $this->arrVisivel,
	'arrOrdem'      => $this->arrOrdem
	);

	if ( !isset( $_SESSION['indice_sessao_combo_popup'] ) )
	{
		$_SESSION['indice_sessao_combo_popup'] = array();
	}
	unset($_SESSION['indice_sessao_combo_popup'][$this->nome]);
	$_SESSION['indice_sessao_combo_popup'][$this->nome] = $dados_sessao;

	// monta html para formulario
	$tamanho    = explode( 'x', $this->tamanho_janela );
	$onclick    = ' onclick="javascript:combo_popup_alterar_campo_busca( this );" ';

	/*** Adiciona a função Javascript ***/
	$this->funcaoJS = (is_null($this->funcaoJS)) ? 'false' : "'" . $this->funcaoJS . "'";

	$ondblclick = ' ondblclick="javascript:combo_popup_abre_janela( \'' . $this->nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$this->funcaoJS.' );" ';
	$ondelete   = ' onkeydown="javascript:combo_popup_remove_selecionados( event, \'' . $nome . '\' );" ';
	$onpop		= ( $this->onpop == null ) ? $this->onpop = '' : ' onpop="' . $this->onpop . '"';
	$onpush		= ( $this->onpush == null ) ? $this->onpush = '' : ' onpush="' . $this->onpush . '"';
	$habilitado_select = $this->habilitado == 'S' ? '' : ' disabled="disabled" ' ;
	$select =
	'<select ' .
	'maximo="'. $this->maximo_itens .'" tipo="combo_popup" title="'. $this->titulo .'" ' .
	'multiple="multiple" size="' . $this->size . '" ' .
	'name="' . $this->nome . '[]" id="' . $this->nome . '" '.
	$onclick . $ondblclick . $ondelete . $onpop . $onpush  .
	'class="CampoEstilo" style="width:' . $this->width . 'px;" ' .
	$habilitado_select .
	'>';
	if($this->value && count( $this->value ) > 0){
		$itens_criados = 0;
		foreach ( $this->value as $item )
		{
			if (is_array($item))
			{
				$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
				$itens_criados++;
				if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
				{
					break;
				}
			}
		}
	} elseif ( ${$this->nome} && count( ${$this->nome} ) > 0 ) {
		$itens_criados = 0;
		if( is_array(${$this->nome}) ){
			foreach ( ${$this->nome} as $item )
			{
				if (is_array($item))
				{
					$select .= '<option value="' . $item['codigo'] . '">' . simec_htmlentities( $item['descricao'] ) . '</option>';
					$itens_criados++;
					if ( $maximo_itens != 0 && $itens_criados >= $maximo_itens )
					{
						break;
					}
				}
			}
		}
	}
	else if ( $this->habilitado == 'S' )
	{
		$select .= '<option value="">Duplo clique para selecionar da lista</option>';
	}
	else
	{
		$select .= '<option value="">Nenhum</option>';
	}
	$select .= '</select>';
	$buscaCodigo = '';

	#Alteração feita por wesley romualdo
	#caso a consulta não seja por descrição e sim por codigo, não permitir digitar string no campo de consulta.
	if($this->campo_busca_descricao == true ){
		$paramentro = "";
		$complOnblur = "";
	} else {
		$paramentro = "onkeyup=\"this.value=mascaraglobal('[#]',this.value);\"";
		$complOnblur = "this.value=mascaraglobal('[#]',this.value);";
	}

	if ( $this->campo_busca_codigo == true && $this->habilitado == 'S' )
	{
		$buscaCodigo .= '<input type="text" id="combopopup_campo_busca_' . $this->nome . '" onkeypress="combo_popup_keypress_buscar_codigo( event, \'' . $this->nome . '\', this.value );" '.$this->paramentro.' onmouseover="MouseOver( this );" onfocus="MouseClick(this);" onmouseout="MouseOut(this);" onblur="MouseBlur(this); '.$complOnblur.'" class="normal" style="margin: 2px 0;" />';
		$buscaCodigo .= '&nbsp;<img title="adicionar" align="absmiddle" src="/imagens/check_p.gif" onclick="combo_popup_buscar_codigo( \'' . $this->nome . '\', document.getElementById( \'combopopup_campo_busca_' . $this->nome . '\' ).value );"/>';
		$buscaCodigo .= '&nbsp;<img title="remover" align="absmiddle" src="/imagens/exclui_p.gif" onclick="combo_popup_remover_item( \'' . $this->nome . '\', document.getElementById( \'combopopup_campo_busca_' . $this->nome . '\' ).value, true );"/>';
		$buscaCodigo .= '&nbsp;<img title="abrir lista" align="absmiddle" src="/imagens/pop_p.gif" onclick="combo_popup_abre_janela( \'' . $this->nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ' );"/>';
		$buscaCodigo .= '<br/>';
	}
	#Fim da alteração realizada por wesley romualdo

	$flagContem = '';
	if ( $this->campo_flag_contem == true )
	{
		$nomeFlagContemGlobal = $this->nome . '_campo_excludente';
		global ${$nomeFlagContemGlobal};
		$flagContem .= '<input type="checkbox" id="' . $this->nome . '_campo_excludente" name="' . $this->nome . '_campo_excludente" value="1" ' . ( ${$nomeFlagContemGlobal} ? 'checked="checked"' : '' ) . ' style="margin:0;" />';
		$flagContem .= '&nbsp;<label for="' . $this->nome . '_campo_excludente">Não contém</label>';
	}
	$cabecalho = '';
	if ( $buscaCodigo != '' || $flagContem != '' )
	{
		$cabecalho .= '<table width="' . $this->width . '" border="0" cellspacing="0" cellpadding="0"><tr>';
		$cabecalho .= '<td align="left">' . $buscaCodigo . '</td>';
		$cabecalho .= '<td align="right">' . $flagContem . '</td>';
		$cabecalho .= '</tr></table>';
	}
	print $cabecalho . $select  . ' <img src="../imagens/pop_p.gif" style="cursor:pointer;" align="absmiddle" onclick="javascript:combo_popup_abre_janela( \'' . $this->nome . '\', ' . $tamanho[0] . ', ' . $tamanho[1] . ', '.$this->funcaoJS.' );">';        
    }
}
