<?hh // strict

namespace Ivyhjk\Xml;

use Exception;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Entity\Param;
use Ivyhjk\Xml\Entity\Params;
use Ivyhjk\Xml\Entity\Struct;
use Ivyhjk\Xml\Entity\MethodCall;
use Ivyhjk\Xml\Entity\MethodName;
use Ivyhjk\Xml\Exception\XmlException;

/**
 * Manage XML RPC requests.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class RPCRequest
{
    /**
     * Encode an XML RPC request.
     *
     * @param string $method RPC method.
     * @param mixed $parameters RPC method args.
     * @param string $encoding The XML encoding.
     *
     * @return string
     * @throws Ivyhjk\Xml\Exception\XmlException
     */
    public static function encode(string $method, mixed $parameters, string $encoding = 'iso-8859-1') : string
    {
        libxml_use_internal_errors(true);

        $document = new DOMDocument('1.0', $encoding);

        $methodName = new MethodName($method, $document);

        $givenParams = Vector{};

        if (is_array($parameters) && array_key_exists(0, $parameters)) {
            foreach ($parameters as $parameter) {
                $value = new Value(Vector{$parameter}, $document);
                $param = new Param(Vector{$value}, $document);

                $givenParams->add($param);
            }
        } else {
            $value = new Value(Vector{$parameters}, $document);
            $param = new Param(Vector{$value}, $document);
            $givenParams = Vector{$param};
        }

        $params = new Params($givenParams, $document);

        $methodCall = new MethodCall($methodName, $params, $document);

        $document->appendChild($methodCall->getElement());

        return $document->saveXML();
    }

    /**
     * Decode an XML RPC request.
     *
     * @param string $xml The XML document to parse.
     * @param string $method The XML RPC method.
     *
     * @return Map<string, mixed>
     * @throws Ivyhjk\Xml\Exception\XmlException
     */
    public static function decode(string $xml) : Map<string, mixed>
    {
        libxml_use_internal_errors(true);

        try {
            $element = new SimpleXMLElement($xml);
        } catch (Exception $e) {
            throw new XmlException($e->getMessage());
        }

        $parameters = Vector{};

        $methodCallEntity = MethodCall::fromNode($element, new DOMDocument());
        $paramsEntity = $methodCallEntity->getParams();
        $paramEntities = $paramsEntity->getParameters();

        foreach ($paramEntities as $paramEntity) {
            $valueEntities = $paramEntity->getValues();

            $parsedValues = Value::parseValues($valueEntities);

            if ($parsedValues->count() === 1) {
                $parameters->add($parsedValues->firstValue());
            } else {
                $parameters->add($parsedValues);
            }
        }

        return Map{
            'method' => $methodCallEntity->getMethodName()->getName(),
            'parameters' => $parameters
        };
    }
}
