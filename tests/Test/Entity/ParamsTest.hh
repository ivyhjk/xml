<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Param;
use Ivyhjk\Xml\Entity\Params;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Test the correct workflow for <params> tag wrapper.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class ParamsTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Test the correct workflow for getElement method.
     *
     * @return void
     */
    public function testGetElement() : void
    {
        $document = new DOMDocument();

        $parameters = Vector{
            new Param(Vector{
                new Value(Vector{'foo'}, $document)
            }, $document),
            new Param(Vector{
                new Value(Vector{'bar'}, $document)
            }, $document)
        };

        $params = new Params($parameters, $document);

        $document->appendChild($params->getElement());

        $xml = \preg_replace('/\n/', '', $document->saveXML());

        $expectedXML = '<?xml version="1.0"?><params><param><value><string>foo</string></value></param><param><value><string>bar</string></value></param></params>';

        static::assertEquals($expectedXML, $xml);
    }

    /**
     * Test the fromNode error.
     *
     * @return void
     */
    public function testFromNodeError() : void
    {
        $node = new SimpleXMLElement('<invalidTag/>');

        $this->expectException(InvalidNodeException::class);
        Params::fromNode($node, new DOMDocument());
    }

    /**
     * Test the fromNode correct workflow.
     *
     * @return void
     */
    public function testFromNode() : void
    {
        $document = new DOMDocument();

        $node = new SimpleXMLElement('
            <params>
                <param>

                </param>
            </params>
        ');

        Params::fromNode($node, new DOMDocument());
    }
}
