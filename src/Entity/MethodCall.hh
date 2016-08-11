<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * <methodCall> tag concrete class.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class MethodCall extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'methodCall';

    /**
     * Description of what this does.
     *
     * @param Ivyhjk\Xml\Entity\MethodName $methodName The associated method name.
     * @param Ivyhjk\Xml\Entity\Params $params The associated parameters wrapper.
     * @param DOMDocument $document The root document.
     *
     * @return void
     */
    public function __construct(
        private MethodName $methodName,
        private Params $params,
        DOMDocument $document
    ) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the associated method name.
     *
     * @return Ivyhjk\Xml\Entity\MethodName
     */
    public function getMethodName() : MethodName
    {
        return $this->methodName;
    }

    /**
     * Get the associated params.
     *
     * @return Ivyhjk\Xml\Entity\Params
     */
    public function getParams() : Params
    {
        return $this->params;
    }

    /**
     * Get the <methodCall> tag as a DOMelement.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $element = $this->getDocument()->createElement('methodCall');

        // Append the method name.
        $element->appendChild($this->getMethodName()->getElement());
        // Append the parameters.
        $element->appendChild($this->getParams()->getElement());

        return $element;
    }

    /**
     * Generate a new MethodCall instance from a given SimpleXMLElement node.
     *
     * @param SimpleXMLElement $node The xml as a SimpleXMLElement node.
     *
     * @return MethodCall
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : MethodCall
    {
        $name = $node->getName();

        if ($name !== self::TAG_NAME) {
            throw new InvalidNodeException();
        }

        $methodNameNodes = new Vector($node->xpath('methodName'));
        $methodNameNode = $methodNameNodes->firstValue();

        if ($methodNameNode === null) {
            throw new InvalidNodeException('Missing node "methodName"');
        }

        $methodNameEntity = MethodName::fromNode($methodNameNode, $document);

        $paramsNodes = new Vector($node->xpath('params'));
        $paramsNode = $paramsNodes->firstValue();

        if ($paramsNode === null) {
            $paramsEntity = new Params(Vector{}, $document);
        } else {
            $paramsEntity = Params::fromNode($paramsNode, $document);
        }

        return new MethodCall($methodNameEntity, $paramsEntity, $document);
    }
}
