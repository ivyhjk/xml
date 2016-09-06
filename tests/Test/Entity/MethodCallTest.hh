<?hh // strict

namespace Ivyhjk\Xml\Test\Entity;

use DOMDocument;
use SimpleXMLElement;
use Ivyhjk\Xml\Entity\Value;
use Ivyhjk\Xml\Entity\Param;
use Ivyhjk\Xml\Entity\Params;
use Ivyhjk\Xml\Entity\MethodCall;
use Ivyhjk\Xml\Entity\MethodName;
use Ivyhjk\Xml\Exception\InvalidNodeException;

/**
 * Test the correct worflow for <methodCall> tag wrapper.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Xml\Test\Entity
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class MethodCallTest extends \PHPUnit_Framework_TestCase
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

        $methodName = new MethodName('MyMethod', $document);

        $methodCall = new MethodCall($methodName, $params, $document);

        $document->appendChild($methodCall->getElement());

        $expectedXml = '<?xml version="1.0"?><methodCall><methodName>MyMethod</methodName><params><param><value><string>foo</string></value></param><param><value><string>bar</string></value></param></params></methodCall>';

        $xml = preg_replace('/\n/', '', $document->saveXML());

        static::assertSame($expectedXml, $xml);
    }

    /**
     * Test the fromNode method when an invalid node is sent.
     *
     * @return void
     */
    public function testFromNodeInvalidNode() : void
    {
        $node = new SimpleXMLElement('<invalidTag/>');

        $this->expectException(InvalidNodeException::class);
        MethodCall::fromNode($node, new DOMDocument());
    }

    /**
     * Test the correct workflow for fromNode method.
     *
     * @return void
     */
    public function testFromNode() : void
    {
        $node = new SimpleXMLElement('
            <methodCall>
                <methodName>MyMethod</methodName>
                <params>
                    <param>
                    </param>
                </params>
            </methodCall>
        ');

        $methodCallEntity = MethodCall::fromNode($node, new DOMDocument());

        $expectedParamsEntity = new Params(Vector{
            new Param(Vector{}, new DOMDocument())
        }, new DOMDocument());

        $expectedMethodNameEntity = new MethodName('MyMethod', new DOMDocument());

        $this->assertTrue(
            $expectedParamsEntity == $methodCallEntity->getParams()
        );

        $this->assertTrue(
            $expectedMethodNameEntity == $methodCallEntity->getMethodName()
        );
    }
}
