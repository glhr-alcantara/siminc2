<?php

include_once APPRAIZ. "includes/connection/connection.php";

/**
 * Classe para estabelecer conex�o e permitir manipular os banco de dados adicionais no sistema
 * 
 * @example : $listaUsuario = adapterConnection::simec()->carregar("SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = ''");
 */
class adapterConnection {

    /**
     * Configuracoes do banco de dados
     * 
     * @var stdclass
     */
    private $config;
    
    /**
     * Banco de dados
     * 
     * @var connection
     */
    private $db;
    
    /**
     * Guarda uma inst�ncia da classe
     * 
     * @var adapterConnection
     */
    private static $instance;
    
    /**
     * Busca configura��es do banco
     * 
     * @return stdClass
     */
    public function getConfig() {
        return $this->config;
    }

    /**
     * Busca banco de dados
     * 
     * @return connection
     */
    public function getDb() {
        return $this->db;
    }

    /**
     * Atribui configura��es
     * 
     * @param stdclass $config
     * @return \adapterConnection
     */
    public function setConfig(stdclass $config) {
        $this->config = $config;
        return $this;
    }

    /**
     * Atribui conex�o do banco de dados
     * 
     * @param connection $db
     * @return \adapterConnection
     */
    public function setDb(connection $db) {
        $this->db = $db;
        return $this;
    }
    
    /**
     * Estabelecer conex�o e permitir manipular os banco de dados adicionais no sistema
     * 
     * @example $listaUsuario = adapterConnection::simec()->carregar("SELECT u.* FROM seguranca.usuario u WHERE u.usucpf = ''");
     */
    private function __construct() {}
    
    /**
     * Retorna a instancia da classe. Caso n�o exista instancia o metodo cria uma nova instancia.
     * 
     * @return adapterConnection
     */
    private static function getInstance(){
        if (!isset(self::$instance)){
            $className = __CLASS__;
            self::$instance = new $className;
        }

        return self::$instance;
    }
    
    /**
     * Carrega conex�o de banco de dados
     * 
     * @return adapterConnection
     */
    private function loadDb(){
        $this->db = connection::getInstance();
        return self::$instance;
    }

    /**
     * Configura e abre conex�o com o banco de dados
     * 
     * @return boolean
     */
    private function open(){
        $resultado = FALSE;

        # Verifica se banco de dados � o mesmo da configura��o atual ou se n�o existe conex�o
        if($this->db->getDbname() != $this->config->dbname || !is_resource($this->db->getLink())){
            # Configura conex�o e estabelece conex�o
            $this->db
                ->setHost($this->config->host)
                ->setPort($this->config->port)
                ->setUser($this->config->user)
                ->setPassword($this->config->password)
                ->setDbname($this->config->dbname);
            
            if(!empty($this->config->searchPath)){
                $this->db->setSearchPath($this->config->searchPath);
            }

            if(!empty($this->config->clientEncoding)){
                $this->db->setClientEncoding($this->config->clientEncoding);
            }
            
            $resultado = $this->db->connect();
        }

        return $resultado;
    }
    
    /**
     * Carrega configura��es de banco atribuidas no arquivo de configura��es gerais do sistema
     * 
     * @param stdclass $globalConfig
     * @return adapterConnection
     */
    private function loadConfig($globalConfig){
        
        if(!$globalConfig instanceof stdClass){
            $globalConfig = (object) array();
        }
        
        $this->setConfig($globalConfig);
        return self::$instance;
    }
    
    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o SIMEC
     *
     * @global stdclass $configDbSimec
     * @return adapterConnection
     */
    public static function sig(){
    	global $configDbSig;
    	 
    	self::getInstance()->loadConfig($configDbSig)->loadDb()->open();
    	return self::$instance;
    }
    
    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o PDDEInterativo
     * 
     * @global stdclass $configDbPddeinterativo
     * @return adapterConnection
     */
    public static function pddeinterativo(){
        global $configDbPddeinterativo;

        self::getInstance()->loadConfig($configDbPddeinterativo)->loadDb()->open();
        return self::$instance;
    }

    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o PDDEInterativo
     *
     * @global stdclass $configDbOldSiminc
     * @return adapterConnection
     */
    public static function oldSiminc(){
        global $configDbOldSiminc;

        self::getInstance()->loadConfig($configDbOldSiminc)->loadDb()->open();
        return self::$instance;
    }

    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o Auditoria
     *
     * @global stdclass $configDbAuditoria
     * @return adapterConnection
     */
    public static function auditoria(){
        global $configDbAuditoria;

        self::getInstance()->loadConfig($configDbAuditoria)->loadDb()->open();
        return self::$instance;
    }
    
    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o dbcoorporativo
     *
     * @global stdclass $configDbCoorporativo
     * @return adapterConnection
     */
    public static function coorporativo(){
    	global $configDbCoorporativo;
    
    	self::getInstance()->loadConfig($configDbCoorporativo)->loadDb()->open();
    	return self::$instance;
    }
    

    /**
     * Configura banco de acordo com as configura��es gerais do sistema para 
     * acessar o banco de dados do SIAFI
     * 
     * @global stdclass $configDbSiafi
     * @return adapterConnection
     */
    public static function siafi(){
        global $configDbSiafi;
        
        self::getInstance()->loadConfig($configDbSiafi)->loadDb()->open();
        return self::$instance;
    }
    
    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o SIGFOR
     * 
     * @global stdclass $configDbSigfor
     * @return adapterConnection
     */
    public static function sigfor(){
        global $configDbSigfor;
        
        self::getInstance()->loadConfig($configDbSigfor)->loadDb()->open();
        return self::$instance;
    }
    
    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o EMEC
     * 
     * @global stdclass $configDbEmec
     * @return adapterConnection
     */
    public static function emec(){
        global $configDbEmec;
        
        self::getInstance()->loadConfig($configDbEmec)->loadDb()->open();
        return self::$instance;
    }

    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o SISTEC
     *
     * @global stdclass $configDbSistec
     * @return adapterConnection
     */
    public static function sistec(){
        global $configDbSistec;

        self::getInstance()->loadConfig($configDbSistec)->loadDb()->open();
        return self::$instance;
    }

    /**
     * Configura banco de acordo com as configura��es gerais do sistema para acessar o Painel P�blico
     *
     * @global stdclass $configDbPainelpublico
     * @return adapterConnection
     */
    public static function painelPublico(){
        global $configDbPainelpublico;

        self::getInstance()->loadConfig($configDbPainelpublico)->loadDb()->open();
        return self::$instance;
    }

    /**
     * Realiza consulta ao banco de dados retornando resultados em lista
     * 
     * @param string $sql Consulta SQL
     * @return array Lista com resultado da consulta
     */
    public function carregar($sql) {
        if(is_resource($this->db->getLink())){
            return $this->db->fetch($sql);
        }
    }

    /**
     * Realiza consulta ao banco de dados retornando todos os dados de um registro
     * 
     * @param string $sql Consulta SQL
     * @return array Lista com resultado da consulta
     */
    public function pegaLinha($sql) {
        if(is_resource($this->db->getLink())){
            return $this->db->fetchRow($sql);
        }
    }
    
    /**
     * Realiza consulta ao banco de dados retornando uma coluna do resultado
     * 
     * @param string $sql Consulta SQL
     * @return array Lista com resultado da consulta
     */
    public function pegaUm($sql) {
        if(is_resource($this->db->getLink())){
            return $this->db->fetchOne($sql);
        }
    }
    
    /**
     * Executa auditoria de consulta ao banco de dados
     * 
     * @param string $sql Consulta SQL
     * @return boolean/resource
     */
    public function auditar($sql) {
        if(is_resource($this->db->getLink())){
            return $this->db->execute($sql, TRUE);
        }
    }
    
    /**
     * Executa SQL(INSERT, UPDATE, DELETE) no banco de dados
     * 
     * @param string $sql
     * @return boolean/resource
     */
    public function executar($sql) {
        if(is_resource($this->db->getLink())){
            return $this->db->execute($sql, TRUE);
        }
    }
    
    /**
     * Ao descarregar o objeto da m�moria encerra a conex�o com o Banco de Dados.
     * 
     * @return VOID
     */
    public function __destruct(){
        $this->db->close();
    }

    /**
     * Impede que o desenvolvedor clone ou copie a inst�ncia evitando objetos 
     * desnecessarios na m�moria do servidor em um mesmo escopo do programa
     * 
     * @return VOID
     */
    public function __clone()
    {
        trigger_error(
            'N�o � permitido clonar ou copiar a inst�ncia da classe de conex�o!',
            E_USER_ERROR);
    }
    
}
