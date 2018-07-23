<?php

class campo_texto{
    public $var;
    public $obrig=false;
    public $habil=true;
    public $label='';
    public $size='';
    public $max='';
    public $masc='';
    public $hid='';
    public $align='left';
    public $txtdica='';
    public $acao=0;
    public $complemento='';
    public $evtkeyup='';
    public $value = null;
    public $evtblur='';
    public $arrStyle = null;
    
    public function gerarComponente(){
	global ${$this->var};

	if (is_array($this->var)) {
		extract($this->var);
		$this->var = $name;
	}
	$this->value = $this->value != '' ? $this->value : ${$this->var};

	if ( $this->obrig == 'S' ) $class = 'obrigatorio';

	$this->arrStyle['text-align'] = $this->align;

	if(is_array($this->arrStyle)){
		$sty = 'style="';
		foreach($this->arrStyle as $chaves=>$dados){
			$sty .= $chaves.':'.$dados.';';
		}
		$sty .= '"';
	}

	if ($this->hid=='S') $dif = '1';
	$texto = '<input type="text" '.$sty.' name="'.$this->var.$dif.'" size="'.($this->size+1).'" maxlength="'.$this->max. '" value="'.$this->value.'"';
	if ($this->masc !="")
	{
		if($this->evtkeyup != "")
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $this->masc . '\',this.value);' . $this->evtkeyup . '"';
		}
		else
		{
			$texto = $texto. ' onKeyUp= "this.value=mascaraglobal(\'' . $this->masc . '\',this.value);"';
		}
	}
	else
	{
		if($this->evtkeyup != "")
		$texto = $texto. ' onKeyUp= "' . $this->evtkeyup . '"';
	}
	$this->habil != 'N' ? $class.=" normal"  :   $class="disabled";

	if ( $this->habil == 'N' )
	{
		if ( !$this->complemento )
		$texto = $texto.' readonly="readonly" style="width:'.($this->size+3).'ex;text-align : '.$this->align.';" ';
		else
		$texto = $texto.' ' . $this->complemento .' readonly="readonly" style="width:'.($this->size+3).'ex;text-align : '.$this->align.';" ';
		//if ($habil == 'N') $texto = $texto.' readonly ';
	}
	else
	{
		$texto .= ' onmouseover="MouseOver(this);';
		if ( $this->txtdica ){
			$texto .= 'return escape(\''.$this->txtdica.'\');';
		}
		if ( !$this->complemento ){
			if( $this->arrStyle ){
				$style = 'style="text-align : '.$this->align.'; width:'.($this->size+3).'ex;"';
			} else {
				$style = $sty;
			}
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);'.($this->masc != '' ? 'this.value=mascaraglobal(\'' . $masc . '\',this.value);' : '') . $evtblur . '" '.$style.' ';
		} else {
			$texto .= '" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);'.($this->masc != '' ? 'this.value=mascaraglobal(\'' . $masc . '\',this.value);' : '') . $evtblur . '" ' . $complemento . ' '.$style.' ';
		}
	}


	$texto = $texto . " title='$this->label' class='$class' />";

	if ($this->obrig == 'S')
            $texto = $texto . obrigatorio();
	if ($hid == 'S'){
		$texto = $texto."<input type='hidden' name='".$this->var."' value ='".$this->value."'>";
	}
	if ($this->acao)
	{
		//  	unidade,unicod,uni,combo,ppaacao,Unidade Orçamentária Responsável, and substr(unicod,1,2) in '26'
		$partes = explode(";", $this->acao);
		$alvo=$partes[0];
		$campo=$partes[1];
		$padrao=$partes[2];
		$tipo=$partes[3];
		$origem=$partes[4];
		$nome_campo=$partes[5];
		$especial=$partes[6];
		$especial2=$partes[7];

		$texto .= '&nbsp;<img border="0" src="../imagens/alterar.gif" title="Editar o campo." onclick='.'"edita_campo('."'$alvo','$campo','$padrao','$tipo','$origem','$nome_campo'";
		//if ($especial)
		$texto .= ",'$especial'";
		//if ($especial2)
		$texto .= ",'$especial2'";
		$texto .= ')">';


	}
	return $texto;        
    }
    
    Function obrigatorio()
    {
            $obrigatorio = " <img border='0' src='". URL_SISTEMA. "imagens/obrig.gif' title='Indica campo obrigatório.' />";
            return $obrigatorio;
    }
}
