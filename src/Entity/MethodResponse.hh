<?hh // strict

namespace Ivyhjk\Xml\Entity;

use DOMElement;
use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * <methodResponse> tag concrete class.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
class MethodResponse extends Tag
{
    /**
     * The XML tag name
     *
     * @var string
     */
    const string TAG_NAME = 'methodResponse';

    /**
     * Generate a new <methodResponse> tag instance.
     *
     * @param Vector<Ivyhjk\Xml\Entity\Params> $params
     * @param DOMDocument $document A root XML node.
     *
     * @return void
     */
    public function __construct(private Vector<Params> $parameters, DOMDocument $document) : void
    {
        parent::__construct($document);
    }

    /**
     * Get the method response params.
     *
     * @return Vector<Ivyhjk\Xml\Entity\Params> $params
     */
    public function getParameters() : Vector<Params>
    {
        return $this->parameters;
    }

    /**
     * Get the tag as a DOMElement object.
     *
     * @return DOMElement
     */
    public function getElement() : DOMElement
    {
        $element = $this
            ->getDocument()
            ->createElement(static::TAG_NAME);

        foreach ($this->getParameters() as $parameter) {
            $element->appendChild($parameter->getElement());
        }

        return $element;
    }

    /**
     * Generate a new MethodResponse from a SimpleXMLElement node.
     *
     * @param SimpleXMLElement $node The node to be parsed.
     * @param DOMDocument $document The root XML node.
     *
     * @return Ivyhjk\Xml\Entity\MethodResponse
     */
    public static function fromNode(SimpleXMLElement $node, DOMDocument $document) : MethodResponse
    {
        $name = $node->getName();

        if ($name !== static::TAG_NAME) {
            throw new InvalidNodeException();
        }

        $paramsNode = $node->xpath(Params::TAG_NAME);

        $parameters = Vector{};

        foreach ($paramsNode as $paramNode) {
            $parameters->add(Params::fromNode($paramNode, $document));
        }

        return new MethodResponse($parameters, $document);
    }
}
