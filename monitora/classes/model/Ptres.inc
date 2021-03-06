<?php
/**
 * Classe de mapeamento da entidade monitora.ptres
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-11-2017
 * @link     no link
 */

require_once APPRAIZ .'includes/classes/Modelo.class.inc';
require_once(APPRAIZ . 'wssof/classes/Importador.inc');
require_once(APPRAIZ . 'wssof/classes/Ws_AcoesDto.inc');
require_once(APPRAIZ . 'monitora/classes/model/Acao.inc');


/**
 * Monitora_Model_Ptres
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release:
 * @link     no link
 */
class Monitora_Model_Ptres extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'monitora.ptres';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ptrid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'acaid' => array('tabela' => 'acao', 'pk' => 'acaid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ptrid' => null,
        'ptres' => null,
        'acaid' => null,
        'ptrano' => null,
        'funcod' => null,
        'sfucod' => null,
        'prgcod' => null,
        'acacod' => null,
        'loccod' => null,
        'unicod' => null,
        'irpcod' => null,
        'ptrdotacao' => null,
        'ptrstatus' => null,
        'ptrdata' => null,
        'plocod' => null,
        'esfcod' => null,
        'ptrdotacaocapital' => null,
        'ptrdotacaocusteio' => null,
        'plodsc' => null,
    );

    public function importarSiop($exercicio, $momento) {
        $exercicio = $exercicio ? $exercicio : date('Y');
        $momento = $momento ? $momento : 3000;

        $map = [
            'retornarAcoes' => 'acoesDTO',
            'retornarLocalizadores' => 'localizadoresDTO',
            'retornarPlanosOrcamentarios' => 'planosOrcamentariosDTO',
        ];

        $mImportador = new Wssof_Importador();
        $mImportador->obterProgramacaoCompleta($exercicio, $momento, $map);
        $this->atualizarFuncionaisSiop($exercicio, $momento);
        $this->commit();
    }

    public function atualizarFuncionaisSiop($exercicio, $momento) {
        $sql = "
            SELECT
                aca.identificadorunico,
                aca.exercicio,
                aca.codigomomento,
                aca.baselegal,
                aca.detalhamentoimplementacao,
                aca.codigoacao,
                aca.codigoorgao,
                aca.codigoprograma,
                aca.codigoobjetivo,
                aca.descricao acadescricao,
                aca.titulo acatitulo,
                loc.codigolocalizador,
                plo.planoorcamentario,
                loc.descricao locdescricao,
                plo.titulo plotitulo
            FROM wssof.ws_acoesdto aca
                JOIN wssof.ws_localizadoresdto loc ON(
                    loc.identificadorunicoacao = aca.identificadorunico
                    AND loc.exercicio = '$exercicio'
                    AND loc.codigomomento = '". (int)$momento. "'
                )
                JOIN wssof.ws_planosorcamentariosdto plo ON(
                    plo.identificadorunicoacao = aca.identificadorunico
                    AND plo.exercicio = '$exercicio'
                    AND plo.codigomomento = '". (int)$momento. "'
                )
            WHERE
                aca.exercicio = '$exercicio'
                AND aca.codigomomento = '". (int)$momento. "'
        ";
        
        $aAcoesDto = $this->carregar($sql);
        $aAcoesDto = $aAcoesDto ? $aAcoesDto : [];

        foreach($aAcoesDto as $acaoDto){
            $acaid = $this->atualizarAcaoSiop($acaoDto);
            $ptrid = $this->atualizarPtresSiop($acaoDto, $acaid);
        }
    }

    public function atualizarAcaoSiop($acaoDto)
    {
        $sql = "
            SELECT
                acaid
            FROM monitora.acao aca
            WHERE
                aca.prgano = '{$acaoDto['exercicio']}'
                AND aca.unicod = '{$acaoDto['codigoorgao']}'
                AND aca.prgcod = '{$acaoDto['codigoprograma']}'
                AND aca.acacod = '{$acaoDto['codigoacao']}'
                AND aca.loccod = '{$acaoDto['codigolocalizador']}'
        ";

        $acaid = $this->pegaUm($sql);

        $mAcao = new Monitora_Model_Acao($acaid);

        $mAcao->ididentificadorunicosiop = $acaoDto['identificadorunico'];
        $mAcao->acatitulo = $mAcao->acadsc = $acaoDto['acatitulo'];
        $mAcao->acadescricao = $acaoDto['acadescricao'];
        $mAcao->acabaselegal = $acaoDto['baselegal'];
        $mAcao->acadetalhamento = $acaoDto['detalhamentoimplementacao'];
        $mAcao->prgano = $acaoDto['exercicio'];
        $mAcao->acacod = $acaoDto['codigoacao'];
        $mAcao->prgcod = $acaoDto['codigoprograma'];
        $mAcao->loccod = $acaoDto['codigolocalizador'];
        $mAcao->saccod = $acaoDto['codigolocalizador'];
        $mAcao->unicod = $acaoDto['codigoorgao'];
        $mAcao->acaobjetivocod = $acaoDto['codigoobjetivo'];

        $mAcao->salvar();
        $acaid = $mAcao->acaid;
        unset($mAcao);

        return $acaid;
    }

    public function atualizarPtresSiop($acaoDto, $acaid)
    {
        $sql = "select ptrid
                from monitora.ptres ptr
                where ptr.ptrano = '{$acaoDto['exercicio']}'
                and ptr.acacod = '{$acaoDto['codigoacao']}'
                and ptr.prgcod = '{$acaoDto['codigoprograma']}'
                and ptr.loccod = '{$acaoDto['codigolocalizador']}'
                and ptr.unicod = '{$acaoDto['codigoorgao']}'
                and ptr.plocod = '{$acaoDto['planoorcamentario']}'
                ";
        $ptrid = $this->pegaUm($sql);

        $mPtres = new Monitora_Model_Ptres($ptrid);

        $mPtres->ptres = 0;
        $mPtres->acaid = $acaid;
        $mPtres->ptrano = $acaoDto['exercicio'];
        $mPtres->prgcod = $acaoDto['codigoprograma'];
        $mPtres->acacod = $acaoDto['codigoacao'];
        $mPtres->loccod = $acaoDto['codigolocalizador'];
        $mPtres->unicod = $acaoDto['codigoorgao'];
        $mPtres->plocod = $acaoDto['planoorcamentario'];
        $mPtres->plodsc = $acaoDto['plotitulo'];

        $mPtres->salvar();
        $ptrid = $mPtres->ptrid;

        unset($mPtres);

        return $ptrid;
    }

    public function recuperarSqlCombo($descricao = null, $where = array(), $order = '') {
        $pk = $this->arChavePrimaria[0];

        foreach (array_keys($this->arAtributos) as $atributo) {
            if (substr($atributo, -6) == 'status') {
                $where[] = "{$atributo} = 'A'";
            } elseif (substr($atributo, -3) == 'ano') {
                $where[] = "$atributo = '{$_SESSION['exercicio']}'";
            }
            if ($descricao) {
                $descricao = implode(" || ' - ' || ", (array)$descricao);
            } else {
                $aPossibilidades = ['dsc', 'desc', 'nome', 'descricao'];
                foreach ($aPossibilidades as $possibilidade) {
                    if (substr($atributo, -(strlen($possibilidade))) == $possibilidade) {
                        $descricao = $atributo;
                        break;
                    }
                }
            }
        }
        $order = $order ? $order : $descricao;
        $where = count($where) ? " where  " . implode(' and ', $where) : '';

        $sql = "
            SELECT
                {$pk} AS codigo,
                {$descricao} AS descricao
            FROM {$this->stNomeTabela}
                JOIN monitora.acao ON ptres.acaid = acao.acaid
                JOIN public.unidadeorcamentaria ON acao.unicod = unidadeorcamentaria.unocod AND acao.prgano = unidadeorcamentaria.prsano
            $where
            ORDER BY
                $order
        ";

        return $sql;
    }

    public function recuperarSqlPorSubunidadeEEnquadramento($suoid, $eqdid)
    {
        $sql = "
            SELECT
                ptr.ptrid codigo,
                coalesce(ptr.prgcod, '') || ' - ' || coalesce(ptr.acacod, '') || ' - ' || coalesce(ptr.loccod, '') || ' - ' || coalesce(ptr.acaobjetivocod, '') || ' - ' || coalesce(ptr.plocod, '') || ' - ' || ptr.acatitulo || ' - ' || ptr.plodsc descricao
            FROM monitora.vw_ptres ptr
                JOIN spo.ptressubunidade psu ON psu.ptrid = ptr.ptrid
                JOIN monitora.enquadramentorp erp ON(
                    erp.irpcod = ptr.irpcod
                    AND erp.eqdid = ". (int)$eqdid. "
                )
            WHERE
                ptr.ptrano = '{$_SESSION['exercicio']}'
                AND ptr.ptrstatus = 'A'
                AND ptr.plocod != 'EIND'
                AND psu.suoid = ". (int)$suoid. "
            ORDER BY
                descricao
        ";
//ver($sql, d);
        return $sql;
    }

    public function recuperarDadosView()
    {
        $sql = "select  * from monitora.vw_ptres ptr 
                where ptr.ptrano = '{$_SESSION['exercicio']}'
                and ptr.ptrstatus = 'A'
                and ptrid = " . (int) $this->ptrid;

        return $this->pegaLinha($sql);
    }

    function recuperarObjetivoPorPtres() {
        global $db;

        $sql = "SELECT o.oppid
                FROM monitora.ptres ptr
                INNER JOIN monitora.acao aca on ptr.acaid = aca.acaid and aca.prgano = ptr.ptrano
                INNER JOIN public.objetivoppa o on o.oppcod = aca.acaobjetivocod and o.prsano = ptr.ptrano
            where ptrid = " . (int) $this->ptrid;

        return $db->pegaUm($sql);
    }

    function recuperarValoresPrePi() {
        global $db;

        $sql = "select sum(pre.plivalorcapital) capitalptres, sum(pre.plivalorcusteio) custeioptres
                from proposta.preplanointerno pre
                where ptrid = " . (int) $this->ptrid;

        return json_encode($db->pegaLinha($sql));
    }

    public function recuperarTodosApoio(stdClass $param) {
        $sql = "
            SELECT DISTINCT
                ptr.ptrid,
                suo.unocod || '(' || suo.unonome || ')' AS unidade_orcamentaria,
                vptr.acacod,
                vptr.acatitulo,
                ptr.ptres,
                vptr.funcional,
                vptr.plocod,
                vptr.plodsc,
                vptr.irpcod,
                ptr.ptrdotacaocusteio,
                ptr.ptrdotacaocapital
            FROM monitora.ptres ptr
                JOIN public.vw_subunidadeorcamentaria suo ON(
                    ptr.unicod = suo.unocod
                    AND ptr.ptrano = suo.prsano
                    AND suo.suostatus = 'A'
                )
                JOIN monitora.vw_ptres vptr ON ptr.ptrid = vptr.ptrid-- SELECT * FROM monitora.vw_ptres 
            WHERE
                ptr.ptrstatus = 'A'
                AND ptr.ptrano = '". (int)$param->exercicio. "'
        ";
        
        $listaResultado = $this->carregar($sql);
        return $listaResultado? $listaResultado: [];
    }

    /**
     * Recupera produtos e unidade de medida da funcional.
     * 
     * @param int $this->ptrano
     * @param int $this->ptrid
     * @global cls_banco $db
     * @return array/boolean
     */
    function recuperarProdutosPorPtres() {
        global $db;
        
        $sql = "
            SELECT DISTINCT
                ptr.ptrid,
                aca.ididentificadorunicosiop,
                pac.descricao produto_acao,
                ppo.descricao produto_po,
                upo.descricao unidade_po,
                uac.descricao unidade_acao,
                aca.acaid,
                aca.acacod,
                aca.locquantidadeproposta
            FROM monitora.ptres ptr
                JOIN monitora.acao aca ON aca.acaid = ptr.acaid
                JOIN wssof.ws_acoesdto a ON(
                    a.identificadorunico = aca.ididentificadorunicosiop::int
                    AND a.exercicio = '".(int)$this->ptrano. "'
                )
                JOIN wssof.ws_localizadoresdto l ON(
                    l.identificadorunicoacao = a.identificadorunico
                    AND l.exercicio = '".(int)$this->ptrano. "'
                )
                JOIN wssof.ws_planosorcamentariosdto po ON(
                    po.identificadorunicoacao = a.identificadorunico
                    AND po.exercicio = '".(int)$this->ptrano. "'
                    AND ptr.plocod = po.planoorcamentario
                )
                LEFT JOIN wssof.ws_produtosdto ppo ON ppo.codigoproduto::text = po.codigoproduto
                LEFT JOIN wssof.ws_produtosdto pac ON pac.codigoproduto::text = a.codigoproduto
                LEFT JOIN wssof.ws_unidadesmedidadto upo ON trim(upo.codigounidademedida)::text = po.codigounidademedida
                LEFT JOIN wssof.ws_unidadesmedidadto uac ON trim(uac.codigounidademedida)::text = a.codigounidademedida
            WHERE
                ptr.ptrid = ". (int)$this->ptrid. "
            ORDER BY
                produto_po,
                produto_acao
        ";
//ver($sql,d);
        return $db->pegaLinha($sql);
    }

    public function salvarQuantidadeLocalizador($locquantidadeproposta = 0)
    {
        $locquantidadeproposta = $locquantidadeproposta ? str_replace(array('.', ','), array('', '.'), $locquantidadeproposta) : 0;
        $sql = "update monitora.acao set locquantidadeproposta = $locquantidadeproposta where acaid = " . (int) $this->acaid;
        $this->executar($sql);
    }


    public function recuperarSubunidades()
    {
        $sql = "select array_to_string(array(
                    select suo.unosigla || ' - ' || suo.suonome suonome
                    from spo.ptressubunidade psu
                            inner join monitora.ptres p on p.ptrid = psu.ptrid
                            inner join public.vw_subunidadeorcamentaria suo on suo.suoid = psu.suoid
                    where p.ptrano = '{$_SESSION['exercicio']}'
                    and p.ptrstatus = 'A'
                    and p.ptrid = " . (int) $this->ptrid . "
                ), ',') as subunidades";

        return $this->pegaUm($sql);
    }

    public function recuperarQuantidadesSiop($momento, $exercicio)
    {
        $sql = "
            SELECT
                'A��o' AS descricao,
                count(*) qtd
            FROM wssof.ws_acoesdto
            WHERE
                exercicio = '". (int)$exercicio. "'
                AND codigomomento = '".(int)$momento. "'

            UNION ALL

            SELECT
                'Localizador' AS descricao,
                count(*) qtd
            FROM wssof.ws_localizadoresdto
            WHERE
                exercicio = '". (int)$exercicio. "'
                AND codigomomento = '".(int)$momento. "'

            UNION ALL

            SELECT
                'Plano Or�ament�rio' AS descricao,
                count(*) qtd
            FROM wssof.ws_planosorcamentariosdto
            WHERE
                exercicio = '". (int)$exercicio. "'
                AND codigomomento = '".(int)$momento. "'";

        return $this->carregar($sql);
    }

    /**
     * Busca o �ltimo momento atualizado.
     * 
     * @param int $exercicio
     * @return int
     */
    public function buscarUltimoMomentoAtualizado($exercicio){
        $sql = "
            SELECT
                aca.codigomomento
            FROM wssof.ws_acoesdto aca
            WHERE
                aca.exercicio = ". (int)$exercicio. "
            ORDER BY
		aca.dataultimaatualizacao DESC
	    LIMIT 1
        ";
        $resultado = $this->pegaUm($sql);
        return $momento = $resultado? $resultado: 0;
    }
    
    public function recuperarFuncionaisSiop($momento, $exercicio)
    {
        $sql = "
            SELECT DISTINCT
                aca.identificadorunico,
                aca.exercicio,
                aca.codigomomento,
                aca.baselegal,
                aca.detalhamentoimplementacao,
                aca.codigoacao,
                aca.codigoorgao,
                aca.codigoprograma,
                aca.codigoobjetivo,
                aca.descricao acadescricao,
                aca.titulo acatitulo,
                loc.codigolocalizador,
                plo.planoorcamentario,
                loc.descricao locdescricao,
                plo.titulo plotitulo
            FROM wssof.ws_acoesdto aca
                JOIN wssof.ws_localizadoresdto loc ON(
                    loc.identificadorunicoacao = aca.identificadorunico
                    AND loc.exercicio = '". (int)$exercicio. "'
                    AND loc.codigomomento = '".(int)$momento. "'
                )
                JOIN wssof.ws_planosorcamentariosdto plo ON(
                    plo.identificadorunicoacao = aca.identificadorunico
                    AND plo.exercicio = '". (int)$exercicio. "'
                    AND plo.codigomomento = '".(int)$momento. "'
                )
            WHERE
                aca.exercicio = '". (int)$exercicio. "'
                AND aca.codigomomento = '".(int)$momento. "'
            ORDER BY
                aca.codigoacao,
                aca.codigoprograma,
                loc.codigolocalizador,
                aca.codigoorgao,
                plo.planoorcamentario
        ";
//ver($sql);
        return $this->carregar($sql);
    }

    public function recuperarComparacaoSiop($momento, $exercicio)
    {
        $sql = "
            SELECT
                'SIOP' AS descricao,
                count(*) qtd
            FROM wssof.ws_acoesdto aca
                JOIN wssof.ws_localizadoresdto loc ON(
                    loc.identificadorunicoacao = aca.identificadorunico
                    AND loc.exercicio = '". (int)$exercicio. "'
                    AND loc.codigomomento = '$momento')
                JOIN wssof.ws_planosorcamentariosdto plo ON(
                    plo.identificadorunicoacao = aca.identificadorunico
                    AND plo.exercicio = '". (int)$exercicio. "'
                    AND plo.codigomomento = '$momento')
            WHERE
                aca.exercicio = '". (int)$exercicio. "'
                AND aca.codigomomento = '$momento'

            UNION ALL

            SELECT
                'Propostas' AS descricao,
                count(*) qtd
            FROM proposta.proposta pro
            WHERE
                pro.prostatus = 'A'
                AND pro.prsano = '". (int)$exercicio. "'
        ";

        return $this->carregar($sql);
    }
}//end Class
?>